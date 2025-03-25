#include "Steve.h"

#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include <iostream>
#include "UI_Mgr.h"

CSteve::CSteve(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CSteve::CSteve(const CSteve& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CSteve::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSteve::Initialize(void* pArg)
{
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
	m_pGameInstance->Add_CollisionGroup(COLLISION_PLAYER, this);

	 //1. 키입력에 따른 이동
	Input_Key(fTimeDelta);
}

void CSteve::Update(_float fTimeDelta)
{
	
	if (FAILED(m_pCollider_CubeCom->Update_ColliderBox()))
	{
		MSG_BOX("Update_ColliderBox()");
		return;
	}

	m_pRigidbodyCom->Update(fTimeDelta, COLLISION_BLOCK);

	//CGameObject* pGameObject;
	//_float fDist;
	//m_pGameInstance->Ray_Cast
	//(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
	//	m_pTransformCom->Get_State(CTransform::STATE_LOOK),
	//	10.f,
	//	COLLISION_PLAYER,
	//	fDist,
	//	&pGameObject
	//);
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

	if (FAILED(m_pCollider_CubeCom->Render_ColliderBox(false)))
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

	Move(fTimeDelta);
}

void CSteve::Move(_float fTimeDelta)
{
	bool isMoving = false;

	CGameObject* collider{ nullptr };
	// 블럭 충돌 여부 확인.	
	collider = m_pGameInstance->Collision_Check_with_Group(
		COLLISION_BLOCK,		
		m_pCollider_CubeCom,	
		CCollider_Manager::COLLSIION_CUBE
		);	

	if (collider != nullptr)
	{
		//m_pParticleSandDestroy->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));		
		CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// 적용 씬.
			PROTOTYPE_GAMEOBJECT_PARTICLE_DASH,	// 가져올 프로토타입.
			LEVEL_STATIC,	// 가져올 씬.
			LAYER_PARTICLE_DASH);	// 애드오브젝트에 추가할 레이어

		if (particle != nullptr)
		{
			/*particle->GetTransform()->Set_State(CTransform::STATE_LOOK, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			particle->GetTransform()->Set_State(CTransform::STATE_UP, m_pTransformCom->Get_State(CTransform::STATE_UP));
			particle->GetTransform()->Set_State(CTransform::STATE_RIGHT, m_pTransformCom->Get_State(CTransform::STATE_RIGHT));*/
			particle->Replay(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			//m_pGameInstance->Pop(particle);
		}
	}
	
	Matrix mat = *m_pTransformCom->Get_WorldMatrix();

	if (m_pGameInstance->Key_Pressing('W'))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_skelAnime->Set_BoneLocalMatrix(0, mat);
		isMoving = true;
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
		if (m_pRigidbodyCom->Jump())
		{
			//m_pGameInstance->Play_Sound("event:/Built_Fail");
		}
		
	}

	/* 현우 Test */
	if (m_pGameInstance->Key_Pressing('Z'))
	{
		CUI_Mgr::Get_Instance()->TakeDamge();
	}
}

HRESULT CSteve::Ready_Components()
{
	// 스티브 텍스처
  /* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Steve"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 4.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
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


	// 본 + 애니메이션
	CSkeletalAnimator::DESC DescSekel = { m_pVIBufferComs };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SkeletalAnimator"),
		TEXT("m_pSkeletalAnimatorCom"), reinterpret_cast<CComponent**>(&m_skelAnime), &DescSekel)))
		return E_FAIL;


	//콜라이더
	/* For.Com_Collider */
	CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
	Desc.fRadiusX = 0.3f; Desc.fRadiusY = 0.8f; Desc.fRadiusZ = 0.3;
	Desc.fOffSetY = 0.8f;
	Desc.pTransformCom = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pCollider_CubeCom), &Desc)))
		return E_FAIL;

	//리지드바디
	/* For.Com_Rigidbody */
	CRigidbody::RIGIDBODY_DESC	RigidbodyDesc{ m_pTransformCom, m_pCollider_CubeCom, 1.f };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Rigidbody"),
		TEXT("Com_Rigidbody"), reinterpret_cast<CComponent**>(&m_pRigidbodyCom), &RigidbodyDesc)))
		return E_FAIL;
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
	KEYFREAME Walk_2_F = { 1.0f, mat2 }; //60
	KEYFREAME Walk_3_F = { 2.0f, mat }; // 0
	KEYFREAME Walk_4_F = { 3.0f, mat3 }; // -60
	KEYFREAME Walk_5_F = { 3.999f, mat }; // 0

	KEYFREAME Walk_1_B = { 0.f,  mat };
	KEYFREAME Walk_2_B = { 1.0f, mat3 };
	KEYFREAME Walk_3_B = { 2.0f, mat };
	KEYFREAME Walk_4_B = { 3.0f, mat2 };
	KEYFREAME Walk_5_B = { 3.999f, mat };

	m_skelAnime->Add_Animation(ANIM_type::Swing_F, Walk_1_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_F, Walk_2_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_F, Walk_3_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_F, Walk_4_F);
	m_skelAnime->Add_Animation(ANIM_type::Swing_F, Walk_5_F);

	m_skelAnime->Add_Animation(ANIM_type::Swing_B, Walk_1_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_B, Walk_2_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_B, Walk_3_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_B, Walk_4_B);
	m_skelAnime->Add_Animation(ANIM_type::Swing_B, Walk_5_B);


/*----------
* IDEL 
------------*/

	mat2 = Matrix();
	mat2.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-3.f));

	mat3 = Matrix();
	mat3.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(3.f));

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

}

void CSteve::Motion_Idle(_float fTimeDelta)
{
	if (m_skelAnime->is_AnimtionEND())
	{
		m_eCurAnim = IDLE;
	}

	/*if (!m_skelAnime->IsBlending() && m_skelAnime->GetCurrentAnim() != ANIM::IDLE)
	{
		m_skelAnime->Start_Blend(WALK_F, IDLE, 0.005f);
		m_skelAnime->Start_Blend(WALK_B, IDLE, 9999999.f);
	}*/

	m_skelAnime->Update_Animetion(INIT, fTimeDelta, 3);
	m_skelAnime->Update_Animetion(INIT, fTimeDelta, 4);
	m_skelAnime->Update_Animetion(Swing_L, fTimeDelta, 5);
	m_skelAnime->Update_Animetion(Swing_R, fTimeDelta, 6);
}

void CSteve::Motion_Walk(_float fTimeDelta)
{
	if (m_skelAnime->is_AnimtionEND())
	{
		m_eCurAnim = WALK;
	}

	m_skelAnime->Update_Animetion(Swing_B, fTimeDelta, 3);
	m_skelAnime->Update_Animetion(Swing_F, fTimeDelta, 4);
	m_skelAnime->Update_Animetion(Swing_F, fTimeDelta, 5);
	m_skelAnime->Update_Animetion(Swing_B, fTimeDelta, 6);

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

	Safe_Release(m_pRigidbodyCom);
	Safe_Release(m_pCollider_CubeCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_skelAnime);
	for (auto& buffer : m_pVIBufferComs)
		Safe_Release(buffer);

}

