#include "Steve.h"

#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Anim_Cube.h"
#include "GameInstance.h"
#include "UI_Mgr.h"

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

	// 기타 스텟 초기화 ( Pawn 에 선언 되어 있음 )
	m_fSpeed = 5.f;
	m_MaxHp = 100.f;
	m_Hp = 100.f;

	m_Coll_Size = { 0.3f, 0.9f, 0.3f };
	m_Coll_Offset = { 0.f, 0.9f, 0.f };

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	if (FAILED(Ready_Bone()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	m_pRigidbodyCom->Set_MoveSpeed(0.1f);
	m_pRigidbodyCom->Set_MaxSpeed(5.f);
	
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

}

void CSteve::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->UpdateListener(GetPos(), m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_UP));

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
		return;

	if (m_pGameInstance->Key_Down(VK_LBUTTON))
	{
		isAttack = true;
	}

	Move(fTimeDelta);
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
		m_skelAnime->Set_BoneLocalMatrix(0, mat);

	}
	if (m_pGameInstance->Key_Pressing('D'))
	{
		vDirection += vRight;
		m_skelAnime->Set_BoneLocalMatrix(0, mat);

	}

	// 방향 벡터 정규화 (대각선 이동 시 속도 보정)
 	if (D3DXVec3Length(&vDirection) > 0)
	{
		D3DXVec3Normalize(&vDirection, &vDirection);
		m_pRigidbodyCom->Move(vDirection);
		m_eCurAnim = WALK;

		if (m_pRigidbodyCom->isGround())
			PlayDashParticle(fTimeDelta);
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

	/* 현우 Test */
	if (m_pGameInstance->Key_Down('Z'))
	{
		Add_Hp(-15);

		CUI_Mgr::Get_Instance()->SetHP();
	}
	if (m_pGameInstance->Key_Down('X'))
	{
		CUI_Mgr::Get_Instance()->PlayerExp_Set();
	}

	if (m_pGameInstance->Key_Down('C'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_APPLE, 1);
	}
	if (m_pGameInstance->Key_Down('V'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_DIRT, 64);
	}
	if (m_pGameInstance->Key_Down('B'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_GRASSDIRT, 1);
	}
	if (m_pGameInstance->Key_Down('N'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_SAPLING, 1);
	}
	if (m_pGameInstance->Key_Down('M'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEMNAME_APPLE, 1);
	}

	if (m_pGameInstance->Key_Down('L'))
	{
		CUI_Mgr::Get_Instance()->ItemCount_Update(ITEM_WEPON_1, 1);
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
		 { "Leg_R",  1,	  MAtrixTranslation(2.f / 16.f,  0.f / 16.f,	0.f),	MAtrixTranslation(2.f / 16.f,	 0,		0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Leg_L" ,  1,  MAtrixTranslation(-2.f / 16.f,  0.f / 16.f,	0.f),	MAtrixTranslation(-2.f / 16.f,     0,	0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
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
	case CSteve::ANIM_END:
		break;
	default:
		break;
	}	

	if (isAttack)
	{
		if (m_skelAnime->is_AnimtionEND(Attack))
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

	_float3 temp = {};
	D3DXVec3Normalize(&temp, &vforce);
	temp *= 5.f;
	temp.y = 5.f;

	m_pRigidbodyCom->Knock_back(temp);
}

const _float4x4& CSteve::GetSoketMatrix(int index)
{
	return m_skelAnime->GetBoneWorldMatrix(index);
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

