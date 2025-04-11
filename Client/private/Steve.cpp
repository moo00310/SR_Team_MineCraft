#include "Steve.h"

#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Anim_Cube.h"
#include "GameInstance.h"
#include "UI_Mgr.h"
#include <iostream>
#include "Sound_Manager.h"

_float g_fScanRange = 0.f;
_float g_fScanTime = 0.f;
_float g_fScanEndTime = 3.f;
_float g_fScanSpeed = 20.f;
_bool g_bIsScan = false;

CSteve::CSteve(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CPawn{ pGraphic_Device }
{
}

CSteve::CSteve(const CSteve& Prototype)
	: CPawn(Prototype)
{
}

HRESULT CSteve::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSteve::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	// 기타 스텟 초기화 ( Pawn 에 선언 되어 있음 )
	m_fSpeed = 5.f;
	m_MaxHp = 100.f;
	m_Hp = 100.f;
	m_fRun_Speed = 8.f;

	m_Coll_Size = { 0.3f, 0.9f, 0.3f };
	m_Coll_Offset = { 0.f, 0.9f, 0.f };

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	if (FAILED(Ready_Bone()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_pRigidbodyCom->Set_Speed(m_fSpeed);

	m_skelAnime->SetFrameCallback(std::bind(&CSteve::FrameCallback, this, std::placeholders::_1, std::placeholders::_2));

	m_sound = 0.8f;
	
	return S_OK;
}

void CSteve::Priority_Update(_float fTimeDelta)
{

	if (m_bGetHit) {
		m_iGetHitFrame++;
		if (m_iGetHitFrame > 15) {
			m_bGetHit = false;
			m_iGetHitFrame = 0;
			m_eColor = RENDERORIGIN;
		}
	}
	//m_pGameInstance->Add_CollisionGroup(COLLISION_PLAYER, this);
	
	//테스트(Add_Collider_CollisionGroup)
	m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_PLAYER, m_pCollider_CubeCom);

	//1. 키입력에 따른 이동
	Input_Key(fTimeDelta);
}

void CSteve::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (g_fScanTime >= g_fScanEndTime)
	{
		g_bIsScan = false;
		g_fScanRange = 0.f;
		g_fScanTime = 0.f;
	}

	if (g_bIsScan == true)
	{
		g_fScanRange += fTimeDelta * g_fScanSpeed;
		g_fScanTime += fTimeDelta;
	}	
}

void CSteve::Late_Update(_float fTimeDelta)
{
	//m_pGameInstance->UpdateListener(GetPos(), m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_UP));

	// 회전 및 애니메이션 상태 변경
	Turn(fTimeDelta);
	Update_State(fTimeDelta);

	// 모델의 루트본 업데이트 (Position 만)
	Matrix matrix = *m_pTransformCom->Get_WorldMatrix();
	m_skelAnime->Update_RootBone(MAtrixTranslation(matrix._41, matrix._42, matrix._43));

	if (m_isRender)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
			return;
	}
}
	
HRESULT CSteve::Render()
{
	__super::Render();

	return S_OK;
}

void CSteve::SetPos(_float3 v3)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, v3);
}

_float3 CSteve::GetPos()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION);
}

void CSteve::Input_Key(_float fTimeDelta)
{
	CURSORINFO tCursorInfo;
	tCursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&tCursorInfo);

	if (tCursorInfo.flags == CURSOR_SHOWING)
	{
		//마우스 보일때 스티브 움직임 속도 없애기
		m_pRigidbodyCom->Set_Velocity({ 0.f, 0.f, 0.f });
		return;
	}

	if (m_pGameInstance->Key_Down(VK_LBUTTON))
	{
		isAttack = true;
	}

	if (!Get_RigidBody()->Get_isKnockBack())
	{
		Move(fTimeDelta);
	}
}

void CSteve::Move(_float fTimeDelta)
{
	Matrix mat = *m_pTransformCom->Get_WorldMatrix();
	_float3 vDirection = { 0.f, 0.f, 0.f };

	// 캐릭터의 로컬 Look (전방) & Right (우측) 벡터 가져오기
	_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_float3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);

	// 정규화
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	if (m_pGameInstance->Key_Pressing('W'))
	{
		vDirection += vLook;
		m_skelAnime->Set_BoneLocalMatrix(0, mat);
	}

	if (m_pGameInstance->Key_Pressing('S'))
	{
		vDirection -= vLook;
		m_skelAnime->Set_BoneLocalMatrix(0, mat);
	}
	if (m_pGameInstance->Key_Pressing('A'))
	{
		vDirection -= vRight;
		mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(-30));
		m_skelAnime->Set_BoneLocalMatrix(0, mat);

	}
	if (m_pGameInstance->Key_Pressing('D'))
	{
		vDirection += vRight;
		mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(30));
		m_skelAnime->Set_BoneLocalMatrix(0, mat);

	}

	// 방향 벡터 정규화 (대각선 이동 시 속도 보정)
 	if (D3DXVec3Length(&vDirection) > 0)
	{
		D3DXVec3Normalize(&vDirection, &vDirection);
		m_pRigidbodyCom->Move(vDirection);

		if (m_isRun) m_eCurAnim = RUN;
		else m_eCurAnim = WALK;	

		if (m_pRigidbodyCom->isGround())
			PlayDashParticle(fTimeDelta * D3DXVec3LengthSq(&m_pRigidbodyCom->Get_Velocity()) * 0.05f); //현재 속도 만큼 빨리 파티클 나오도록
	}
	else
	{
		ResetDashParticle();
		m_eCurAnim = IDLE;

		if(!m_pRigidbodyCom->Get_isKnockBack())
			m_pRigidbodyCom->StopMovement();
	}

	if (m_pGameInstance->Key_Down(VK_SPACE))
	{
		if (m_pRigidbodyCom->Jump(6.5f))
		{
			//m_pGameInstance->Play_Sound("event:/Built_Fail");
		}
	}

	if (m_pGameInstance->Key_Down(VK_LCONTROL))
	{
		if (m_isRun)
		{
			m_pRigidbodyCom->Set_Speed(m_fSpeed);
			m_isRun = false;
		}
		else
		{
			m_pRigidbodyCom->Set_Speed(m_fRun_Speed);
			m_isRun = true;
		}
	}

	//달리는 중에 w를 안누르고 있으면 달리기 해제
	if (m_isRun)
	{
		if (!m_pGameInstance->Key_Pressing('W'))
		{
			m_pRigidbodyCom->Set_Speed(m_fSpeed);
			m_isRun = false;
		}
	}


	/* 현우 Test */
	if (m_pGameInstance->Key_Down('Z'))
	{
		Add_Hp(-15);

		CUI_Mgr::Get_Instance()->SetHP();
	}
	if (m_pGameInstance->Key_Down('X'))
	{
		CUI_Mgr::Get_Instance()->PlayerExp_Set();
		g_bIsScan = true;
	}

	if (m_pGameInstance->Key_Down('C'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_FURANCE, 1);
	}
	if (m_pGameInstance->Key_Down('V'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_OAKPLANKS, 1);
	}
	if (m_pGameInstance->Key_Down('B'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_WOOD, 1);
	}
	if (m_pGameInstance->Key_Down('N'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_STONE_PICKAXE, 1);
	}
	if (m_pGameInstance->Key_Down('M'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_STEEL_SWORD, 1);
	}

	if (m_pGameInstance->Key_Down('L'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEM_WEPON_1, 1);
	}
	if (m_pGameInstance->Key_Down('K'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_APPLE, 1);
	}
	if (m_pGameInstance->Key_Down('J'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_STICK, 1);
	}
	if (m_pGameInstance->Key_Down('H'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_ROTTENFLESH, 1);
	}

	if (m_pGameInstance->Key_Down('U'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_TORCH, 1);
	}

	if (m_pGameInstance->Key_Down('I'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_FURANCE, 1);
	}

	if (m_pGameInstance->Key_Down('O'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_COAL, 1);
	}

	if (m_pGameInstance->Key_Down('P'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_RAWIRON, 1);
	}
}

HRESULT CSteve::Ready_Components()
{
	// 스티브 텍스처
  /* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Steve"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	m_pVIBufferComs.resize(6);
	// 몸통
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Body"),
		TEXT("m_pVIBufferCom_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[0]))))
		return E_FAIL;

	// 머리
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Head"),
		TEXT("m_pVIBufferCom_Head"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[1]))))
		return E_FAIL;

	// 다리
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Foot_R"),
		TEXT("m_pVIBufferCom_Foot_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[2]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Foot_L"),
		TEXT("m_pVIBufferCom_Foot_L"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[3]))))
		return E_FAIL;

	// 팔
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Arm_R"),
		TEXT("m_pVIBufferCom_Arm_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[4]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Arm_L"),
		TEXT("m_pVIBufferCom_Arm_L"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[5]))))
		return E_FAIL;

	__super::Ready_Components();

	return S_OK;
}

HRESULT CSteve::Ready_Bone()
{
	BONE bone[9] =
	{
		 { "Root"  , -1,  MAtrixTranslation(0.f, 0.f,0.f),	MAtrixTranslation(0.f	,0.f,	0.f),	Matrix(), Matrix() },  // root
		 { "Pelvis",  0,  MAtrixTranslation(0.f,  12.f / 16.f,0.f),	MAtrixTranslation(0.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, 6.f / 16.f, 0.f)},
		 { "Neck"  ,  1,  MAtrixTranslation(0.f,  12.f / 16.f,0.f),	MAtrixTranslation(0.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, 4.f / 16.f, 0.f)},
		 { "Leg_R",  0,	  MAtrixTranslation(2.f / 16.f,  12.f / 16.f,	0.f),	MAtrixTranslation(2.f / 16.f,	  12.f / 16.f,		0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Leg_L" ,  0,  MAtrixTranslation(-2.f / 16.f,  12.f / 16.f,	0.f),	MAtrixTranslation(-2.f / 16.f,     12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Arm_R" ,  1,  MAtrixTranslation(6.f / 16.f,  12.f / 16.f,	0.f),	MAtrixTranslation(6.f / 16.f,   12.f / 16.f	,0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Arm_L" ,  1,  MAtrixTranslation(-6.f / 16.f,  12.f / 16.f,	0.f),	MAtrixTranslation(-6.f / 16.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Soket_R" ,  5,  MAtrixTranslation(0.f,  -12.f / 16.f,	7.f / 16.f),	MAtrixTranslation(0.f,  -12.f / 16.f,	7.f / 16.f), Matrix(), Matrix()},
		 { "Soket_L" ,  6,  MAtrixTranslation(0.f,  -12.f / 16.f,	7.f / 16.f),	MAtrixTranslation(0.f,  -12.f / 16.f,	7.f / 16.f), Matrix(), Matrix()}
	};

	for (int i = 0; i < 9; i++)
	{
		m_skelAnime->Add_Bone(bone[i]);
	}

	return S_OK;
}

HRESULT CSteve::Ready_Animation()
{
	/*----------
	* INIT 모션
	------------*/
	Matrix mat = {};
	KEYFREAME Init = { 0.f, mat };
	m_skelAnime->Add_Animation(ANIM_type::INIT, Init);

	/*----------
	* Walk 모션
	------------*/
	Matrix mat2 = {};
	mat2.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(60.f));

	Matrix mat3 = {};
	mat3.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-60.f));
	
	KEYFREAME Walk_1_F = { 0.f, mat }; //0
	KEYFREAME Walk_2_F = { 0.5f, mat2 }; //60
	KEYFREAME Walk_3_F = { 1.0f, mat }; // 0
	KEYFREAME Walk_4_F = { 1.5f, mat3 }; // -60
	KEYFREAME Walk_5_F = { 2.0f, mat }; // 0

	KEYFREAME Walk_1_B = { 0.f,  mat };
	KEYFREAME Walk_2_B = { 0.5f, mat3 };
	KEYFREAME Walk_3_B = { 1.0f, mat };
	KEYFREAME Walk_4_B = { 1.5f, mat2 };
	KEYFREAME Walk_5_B = { 2.f, mat };

	m_skelAnime->Add_Animation(ANIM_type::Swing_FF, Walk_1_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_FF, Walk_2_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_FF, Walk_3_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_FF, Walk_4_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_FF, Walk_5_F);

	m_skelAnime->Add_Animation(ANIM_type::Swing_FA, Walk_1_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_FA, Walk_2_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_FA, Walk_3_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_FA, Walk_4_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_FA, Walk_5_F);

	m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_1_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_2_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_3_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_4_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_5_B);

	m_skelAnime->Add_Animation(ANIM_type::Swing_BA, Walk_1_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_BA, Walk_2_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_BA, Walk_3_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_BA, Walk_4_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_BA, Walk_5_B);

/*----------
* Walk 모션
------------*/
	Walk_1_F = { 0.f,	mat }; //0
	Walk_2_F = { 0.25f, mat2 }; //60
	Walk_3_F = { 0.5f,	mat }; // 0
	Walk_4_F = { 0.75f, mat3 }; // -60
	Walk_5_F = { 1.0f,	mat }; // 0

	Walk_1_B = { 0.f,	 mat };
	Walk_2_B = { 0.25f,  mat3 };
	Walk_3_B = { 0.5f,	 mat };
	Walk_4_B = { 0.75f,  mat2 };
	Walk_5_B = { 1.0f,	 mat };

	m_skelAnime->Add_Animation(ANIM_type::Run_FF, Walk_1_F);
	m_skelAnime->Add_Animation(ANIM_type::Run_FF, Walk_2_F);
	m_skelAnime->Add_Animation(ANIM_type::Run_FF, Walk_3_F);
	m_skelAnime->Add_Animation(ANIM_type::Run_FF, Walk_4_F);
	m_skelAnime->Add_Animation(ANIM_type::Run_FF, Walk_5_F);
										  
	m_skelAnime->Add_Animation(ANIM_type::Run_FA, Walk_1_F);
	m_skelAnime->Add_Animation(ANIM_type::Run_FA, Walk_2_F);
	m_skelAnime->Add_Animation(ANIM_type::Run_FA, Walk_3_F);
	m_skelAnime->Add_Animation(ANIM_type::Run_FA, Walk_4_F);
	m_skelAnime->Add_Animation(ANIM_type::Run_FA, Walk_5_F);
										  
	m_skelAnime->Add_Animation(ANIM_type::Run_BF, Walk_1_B);
	m_skelAnime->Add_Animation(ANIM_type::Run_BF, Walk_2_B);
	m_skelAnime->Add_Animation(ANIM_type::Run_BF, Walk_3_B);
	m_skelAnime->Add_Animation(ANIM_type::Run_BF, Walk_4_B);
	m_skelAnime->Add_Animation(ANIM_type::Run_BF, Walk_5_B);
										  
	m_skelAnime->Add_Animation(ANIM_type::Run_BA, Walk_1_B);
	m_skelAnime->Add_Animation(ANIM_type::Run_BA, Walk_2_B);
	m_skelAnime->Add_Animation(ANIM_type::Run_BA, Walk_3_B);
	m_skelAnime->Add_Animation(ANIM_type::Run_BA, Walk_4_B);
	m_skelAnime->Add_Animation(ANIM_type::Run_BA, Walk_5_B);

/*----------
* IDEL 
------------*/

	mat2 = Matrix();
	mat2.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-5.f));

	mat3 = Matrix();
	mat3.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(5.f));

	KEYFREAME IDLE1_R = { 0.f, mat };
	KEYFREAME IDLE2_R = { 5.f, mat2 };
	KEYFREAME IDLE3_R = { 9.999f, mat };

	KEYFREAME IDLE1_L = { 0.f, mat };
	KEYFREAME IDLE2_L = { 5.f, mat3 };
	KEYFREAME IDLE3_L = { 9.999f, mat };

	m_skelAnime->Add_Animation(ANIM_type::Swing_R, IDLE1_R);
	m_skelAnime->Add_Animation(ANIM_type::Swing_R, IDLE2_R);
	m_skelAnime->Add_Animation(ANIM_type::Swing_R, IDLE3_R);

	m_skelAnime->Add_Animation(ANIM_type::Swing_L, IDLE1_L);
	m_skelAnime->Add_Animation(ANIM_type::Swing_L, IDLE2_L);
	m_skelAnime->Add_Animation(ANIM_type::Swing_L, IDLE3_L);

	/*----------
	* Attack 모션
	------------*/
	mat2 = {};
	mat2.Turn_Radian(_float3(-1.f, 0.f, 0.f), D3DXToRadian(80));
	mat2.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(30));

	mat3 = {};
	mat3.Turn_Radian(_float3(-1.f, 0.f, 0.f), D3DXToRadian(60));
	mat3.Turn_Radian(_float3(0.f, -1.f, 0.f), D3DXToRadian(30));

	Matrix mat4 = {};
	mat4.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(10));
	mat4.Turn_Radian(_float3(0.f, -1.f, 0.f), D3DXToRadian(30));

	KEYFREAME Attack_1 = { 0.f, mat };
	KEYFREAME Attack_2 = { 0.1f, mat2 };
	KEYFREAME Attack_3 = { 0.2f, mat3 };
	KEYFREAME Attack_4 = { 0.3f, mat4 };
	KEYFREAME Attack_5 = { 0.4f, mat };

	m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_1);
	m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_2);
	m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_3);
	m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_4);
	m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_5);


	/*----------
	* Wepon Attack_Near 모션
	------------*/
	mat2 = {};
	mat2.Turn_Radian(_float3(-0.6f, 1.f, 0.2f), D3DXToRadian(45));

	mat4 = {};
	mat4.Turn_Radian(_float3(-0.6f, 1.f, 0.2f), D3DXToRadian(-45));

	mat3 = {};
	mat3.Turn_Radian(_float3(1.f, -0.5f, 0.25f), D3DXToRadian(-120));

	Matrix mat5 = {};
	mat5.Turn_Radian(_float3(1.f,0.f, 0.f), D3DXToRadian(30));

	Matrix mat6 = {};
	mat6.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(50));

	Matrix mat7 = {};
	mat7.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-30));


	KEYFREAME Attack_Pelvis_Near_1 = { 0.f, mat };
	KEYFREAME Attack_Pelvis_Near_2 = { 0.3f, mat2 };
	KEYFREAME Attack_Pelvis_Near_3 = { 0.5f, mat4 };
	KEYFREAME Attack_Pelvis_Near_4 = { 0.8f, mat };

	KEYFREAME Attack_ArmR_Near_1 = { 0.f, mat };
	KEYFREAME Attack_ArmR_Near_2 = { 0.3f, mat3 };
	KEYFREAME Attack_ArmR_Near_3 = { 0.5f, mat5 };
	KEYFREAME Attack_ArmR_Near_4 = { 0.8f, mat };

	KEYFREAME Attack_ArmL_Near_1 = { 0.f, mat };
	KEYFREAME Attack_ArmL_Near_2 = { 0.3f, mat3 };
	KEYFREAME Attack_ArmL_Near_3 = { 0.5f,mat5 };
	KEYFREAME Attack_ArmL_Near_4 = { 0.8f,mat };

	KEYFREAME Attack_Wepon_Near_1 = { 0.f, mat };
	KEYFREAME Attack_Wepon_Near_2 = { 0.3f, mat6 };
	KEYFREAME Attack_Wepon_Near_3 = { 0.5f,mat6 };
	KEYFREAME Attack_Wepon_Near_4 = { 0.8f,mat };


	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_Pelvis, Attack_Pelvis_Near_1);
	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_Pelvis, Attack_Pelvis_Near_2);
	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_Pelvis, Attack_Pelvis_Near_3);

	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_ArmR, Attack_ArmR_Near_1);
	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_ArmR, Attack_ArmR_Near_2);
	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_ArmR, Attack_ArmR_Near_3);

	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_ArmL, Attack_ArmL_Near_1);
	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_ArmL, Attack_ArmL_Near_2);
	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_ArmL, Attack_ArmL_Near_3);

	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_Wepon, Attack_Wepon_Near_1);
	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_Wepon, Attack_Wepon_Near_2);
	m_skelAnime->Add_Animation(ANIM_type::Wepon_Near_Attack_Wepon, Attack_Wepon_Near_3);

	return S_OK;
}

void CSteve::Update_State(_float fTimeDelta)
{
	if (m_eRreAnim != m_eCurAnim)
	{
		m_skelAnime->Set_ZeroAnimTime();
		m_eRreAnim = m_eCurAnim;
	}

	switch (m_eCurAnim)
	{
	case CSteve::IDLE:
		Motion_Idle(fTimeDelta);
		break;
	case CSteve::WALK:
		Motion_Walk(fTimeDelta);
		break;
	case CSteve::RUN:
		Motion_Run(fTimeDelta);
		break;
	case CSteve::ANIM_END:
		break;
	default:
		break;
	}	

	if (isAttack)
	{
		ITEMNAME eCurItem = CUI_Mgr::Get_Instance()->GetItemTypeName();
		if(eCurItem == ITEM_WEPON_1) Motion_Wepon_Attack(fTimeDelta);
		else Motion_Attack(fTimeDelta);
		
	}
}

void CSteve::Motion_Idle(_float fTimeDelta)
{
	if (m_skelAnime->is_AnimtionEND(INIT) &&
		m_skelAnime->is_AnimtionEND(Swing_L) &&
		m_skelAnime->is_AnimtionEND(Swing_R))
	{
		m_eCurAnim = IDLE;
	}

	m_skelAnime->Update_Animetion(INIT, fTimeDelta, 3);
	m_skelAnime->Update_Animetion(INIT, fTimeDelta, 4);
	m_skelAnime->Update_Animetion(Swing_R, fTimeDelta, 6);
	if (!isAttack)
		m_skelAnime->Update_Animetion(Swing_L, fTimeDelta, 5);
}

void CSteve::Motion_Walk(_float fTimeDelta)
{
	if (m_skelAnime->is_AnimtionEND(Swing_BF) &&
		m_skelAnime->is_AnimtionEND(Swing_FF) &&
		m_skelAnime->is_AnimtionEND(Swing_BA) &&
		m_skelAnime->is_AnimtionEND(Swing_FA)
		)
	{
		m_eCurAnim = WALK;
	}

	m_skelAnime->Update_Animetion(Swing_BF, fTimeDelta, 3);
	m_skelAnime->Update_Animetion(Swing_FF, fTimeDelta, 4);
	m_skelAnime->Update_Animetion(Swing_BA, fTimeDelta, 6);
	if(!isAttack)
		m_skelAnime->Update_Animetion(Swing_FA, fTimeDelta, 5);
}

void CSteve::Motion_Run(_float fTimeDelta)
{
	if (m_skelAnime->is_AnimtionEND(Run_BF) &&
		m_skelAnime->is_AnimtionEND(Run_FF) &&
		m_skelAnime->is_AnimtionEND(Run_BA) &&
		m_skelAnime->is_AnimtionEND(Run_FA)
		)
	{
		m_eCurAnim = RUN;
	}

	m_skelAnime->Update_Animetion(Run_BF, fTimeDelta, 3);
	m_skelAnime->Update_Animetion(Run_FF, fTimeDelta, 4);
	m_skelAnime->Update_Animetion(Run_BA, fTimeDelta, 6);
	if (!isAttack)
		m_skelAnime->Update_Animetion(Run_FA, fTimeDelta, 5);
}

void CSteve::Motion_Attack(_float fTimeDelta)
{
	if (m_skelAnime->is_AnimtionEND(Attack)
		)
	{
		// 캐릭터 레이어 걸려있으면 어택 반복
		if (!m_isAttackContinue)
		{
			isAttack = false;
			m_skelAnime->Reset_fElapsedTime(Swing_FA, Swing_BA);
		}
	}
	m_skelAnime->Update_Animetion(Attack, fTimeDelta, 5);
}

void CSteve::Motion_Wepon_Attack(_float fTimeDelta)
{
	if (m_skelAnime->is_AnimtionEND(Wepon_Near_Attack_Pelvis) &&
		m_skelAnime->is_AnimtionEND(Wepon_Near_Attack_ArmR) &&
		m_skelAnime->is_AnimtionEND(Wepon_Near_Attack_ArmL)&&
		m_skelAnime->is_AnimtionEND(Wepon_Near_Attack_Wepon) 
		)
	{
		// 캐릭터 레이어 걸려있으면 어택 반복
		if (!m_isAttackContinue)
		{
			isAttack = false;
			m_skelAnime->Reset_fElapsedTime(Swing_FA, Swing_BA);
		}
	}

	m_skelAnime->Update_Animetion(Wepon_Near_Attack_Pelvis, fTimeDelta, 1);
	m_skelAnime->Update_Animetion(Wepon_Near_Attack_ArmR, fTimeDelta, 5);
	m_skelAnime->Update_Animetion(Wepon_Near_Attack_ArmL, fTimeDelta, 6);
	m_skelAnime->Update_Animetion(Wepon_Near_Attack_Wepon, fTimeDelta, 7);
}

void CSteve::Turn(_float fTimeDelta)
{
	// 역행렬을 가져와서 머리 회전
	Matrix		mat = {};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &mat);
	D3DXMatrixInverse(&mat, nullptr, &mat);
	mat.Set_State(mat.STATE_POSITION, _float3(0.f, 0.f, 0.f));

	// 본(Neck) 회전
	m_skelAnime->Set_BoneLocalMatrix(2, mat);

	// 몸(Root) 회전
	m_skelAnime->IkLookAt(fTimeDelta, 0, 2);
}


void CSteve::PlayDashParticle(_float fTimeDelta)
{
	// 대시 파티클.
	if (m_IsDashCoolTime == false)
	{
		CParticleEventManager::Get_Instance()->OnParticle(
			PROTOTYPE_GAMEOBJECT_PARTICLE_DASH,
			m_pTransformCom,
			0.2f
		);

		// 쿨타임 진행 처리.
		m_IsDashCoolTime = true;
	}

	// 파티클 시간 증가.
	m_fCurrentDashTime += fTimeDelta;

	if (m_fCurrentDashTime >= m_fCoolTimeDash)
	{
		// 파티클 쿨타임 초기화.
		m_fCurrentDashTime = 0.f;
		m_IsDashCoolTime = false;
	}
}

void CSteve::ResetDashParticle()
{
	// 파티클 쿨타임 초기화.
	m_fCurrentDashTime = 0.f;
	m_IsDashCoolTime = false;
}

void CSteve::Knock_back(const _float3& vforce)
{
	m_eColor = RENDERATTACKED;
	m_bGetHit = true;

	//_float3 temp = {};
	//D3DXVec3Normalize(&temp, &vforce);
	//temp *= 5.f;
	//temp.y = 5.f;

	m_pRigidbodyCom->Knock_back(vforce);
}

const _float4x4& CSteve::GetSoketMatrix(int index)
{
	return m_skelAnime->GetBoneWorldMatrix(index);
}

void CSteve::Add_Hp(_float fAmount)
{
	m_Hp += fAmount;
	CUI_Mgr::Get_Instance()->SetHP();

	m_pGameInstance->Play_Sound(TEXT("Player_Hurt_Old"), SOUND_HIT, this, 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	if (m_Hp <= 0.f) 
	{
		Dead_Pawn();

		//죽으면 바로 리스폰
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3{ 10.f, 20.f, 10.f });
		//m_Hp = 100.f;
		//CUI_Mgr::Get_Instance()->SetHP(); //피가 왜 안차지?
	}
}

void CSteve::Dead_Pawn()
{
	//CPawn::Dead_Pawn();
}

CSteve* CSteve::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSteve* pInstance = new CSteve(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSteve");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSteve::Clone(void* pArg)
{
	CSteve* pInstance = new CSteve(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSteve");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSteve::Free()
{
	__super::Free();

}

void CSteve::FrameCallback(int animType, int frame)
{
	if (animType == Swing_FF && frame == 0)
	{
		m_pGameInstance->Play_Sound(TEXT("Player_Walk_Grass1"), SOUND_WALK1,this, m_sound - 0.35, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
	if (animType == Swing_FF && frame == 1)
	{
		m_pGameInstance->Play_Sound(TEXT("Player_Walk_Grass2"), SOUND_WALK2, this, m_sound - 0.35, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
	if (animType == Swing_FF && frame == 2)
	{
		m_pGameInstance->Play_Sound(TEXT("Player_Walk_Grass2"), SOUND_WALK3, this, m_sound - 0.35, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
	if (animType == Swing_FF && frame == 3)
	{
		m_pGameInstance->Play_Sound(TEXT("Player_Walk_Grass4"), SOUND_WALK4, this, m_sound - 0.35, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	}
}

