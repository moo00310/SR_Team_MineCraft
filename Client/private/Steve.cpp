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
	
	return S_OK;
}

void CSteve::Priority_Update(_float fTimeDelta)
{
	//m_pGameInstance->Add_CollisionGroup(COLLISION_PLAYER, this);
	
	//테스트(Add_Collider_CollisionGroup)
	m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_PLAYER, m_pCollider_CubeCom);

}

void CSteve::Update(_float fTimeDelta)
{
	
	//1. 키입력에 따른 이동
	Input_Key(fTimeDelta);

	if (FAILED(m_pCollider_CubeCom->Update_Collider()))
	{
		MSG_BOX("Update_Collider()");
		return;
	}

	m_pRigidbodyCom->Update(fTimeDelta, COLLISION_BLOCK);
}

void CSteve::Late_Update(_float fTimeDelta)
{
	// 회전 및 애니메이션 상태 변경
	Turn(fTimeDelta);
	Update_State(fTimeDelta);

	// 모델의 루트본 업데이트 (Position 만)
	Matrix matrix = *m_pTransformCom->Get_WorldMatrix();
	m_skelAnime->Update_RootBone(MAtrixTranslation(matrix._41, matrix._42, matrix._43));

	// f5로 랜더 그룹 변경
	if (m_pGameInstance->Key_Down(VK_F5))
	{
		m_bisTPS *= -1;
	}
	if (m_bisTPS > 0)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
			return;
	}
}
		

HRESULT CSteve::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	for (int i = 0; i < 6; i++)
	{
		if (FAILED(m_pVIBufferComs[i]->Bind_WorldMatrix()))
			return E_FAIL;

		if (FAILED(m_pVIBufferComs[i]->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferComs[i]->Render()))
			return E_FAIL;
	}

	if (FAILED(m_pCollider_CubeCom->Render_Collider(true)))
		return E_FAIL;

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
	bool isMoving = false;

	CGameObject* collider{ nullptr };
	////블럭 충돌 여부 확인.	
	//collider = m_pGameInstance->Collision_Check_with_Group(
	//	COLLISION_BLOCK,		
	//	m_pColliderCom,	
	//	CCollider_Manager::COLLSIION_CUBE
	//	);		
	
	Matrix mat = *m_pTransformCom->Get_WorldMatrix();

	if (m_pGameInstance->Key_Pressing('W'))
	{		
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_skelAnime->Set_BoneLocalMatrix(0, mat);
		isMoving = true;

		if (m_pRigidbodyCom->isGround())
		{
			PlayDashParticle(fTimeDelta);
		}		
	}
	if (m_pGameInstance->Key_Up('W'))
	{
		ResetDashParticle();
	}

	if (m_pGameInstance->Key_Pressing('S'))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
		m_skelAnime->Set_BoneLocalMatrix(0, mat);
		isMoving = true;
	}
	if (m_pGameInstance->Key_Pressing('A'))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
		mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(-45));
		m_skelAnime->Set_BoneLocalMatrix(0, mat);
		isMoving = true;
	}
	if (m_pGameInstance->Key_Pressing('D'))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
		mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(45));
		m_skelAnime->Set_BoneLocalMatrix(0, mat);
		isMoving = true;
	}

	m_eCurAnim = isMoving ? WALK : IDLE;

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
	BONE bone[7] =
	{
		 { "Root"  , -1,  MAtrixTranslation(0.f, 0.f,0.f),	MAtrixTranslation(0.f	,0.f,	0.f),	Matrix(), Matrix() },  // root
		 { "Pelvis",  0,  MAtrixTranslation(0.f,  12.f / 16.f,0.f),	MAtrixTranslation(0.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, 6.f / 16.f, 0.f)},
		 { "Neck"  ,  1,  MAtrixTranslation(0.f,  12.f / 16.f,0.f),	MAtrixTranslation(0.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, 4.f / 16.f, 0.f)},
		 { "Leg_R",  1,	  MAtrixTranslation(2.f / 16.f,  0.f / 16.f,	0.f),	MAtrixTranslation(2.f / 16.f,	 0,		0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Leg_L" ,  1,  MAtrixTranslation(-2.f / 16.f,  0.f / 16.f,	0.f),	MAtrixTranslation(-2.f / 16.f,     0,	0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Arm_R" ,  1,  MAtrixTranslation(6.f / 16.f,  12.f / 16.f,	0.f),	MAtrixTranslation(6.f / 16.f,   12.f / 16.f	,0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Arm_L" ,  1,  MAtrixTranslation(-6.f / 16.f,  12.f / 16.f,	0.f),	MAtrixTranslation(-6.f / 16.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
	};

	for (int i = 0; i < 7; i++)
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
			isAttack = false;
			m_skelAnime->Reset_fElapsedTime(Swing_FA, Swing_BA);
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
		CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// 적용 씬.
			PROTOTYPE_GAMEOBJECT_PARTICLE_DASH,	// 가져올 프로토타입.
			LEVEL_STATIC,		// 가져올 씬.
			LAYER_PARTICLE);	// 애드오브젝트에 추가할 레이어

		// 파티클 풀 객체 null 체크.
		if (particle == nullptr)
		{
			return;
		}

		// 파티클 적용.
		//particle->GetTransform()->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		//particle->GetTransform()->Set_State(CTransform::STATE_UP, m_pTransformCom->Get_State(CTransform::STATE_UP));
		//particle->GetTransform()->Set_State(CTransform::STATE_RIGHT, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		//
		//// 플레이어 발에 생성되는데 이러면 블럭에 가려져 파티클이 안나올 수 있다. 약간 y를 올린다.
		//_float3 _pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		//_pos.y += 0.2f;
		//particle->Replay(_pos);

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
	_float3 temp = {};
	D3DXVec3Normalize(&temp, &vforce);
	temp *= 3.f;
	temp.y = 4.f;

	m_pRigidbodyCom->Knock_back(temp);
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

