#include "Rect_Model.h"
#include <iostream>

CRect_Model::CRect_Model(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CRightHand_Object{ pGraphic_Device }
{
}

CRect_Model::CRect_Model(const CRect_Model& Prototype)
	: CRightHand_Object(Prototype)
{
}

HRESULT CRect_Model::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRect_Model::Initialize(void* pArg)
{
	m_RederID = 2;

	__super::Initialize(pArg);	

	return S_OK;
}

void CRect_Model::Priority_Update(_float fTimeDelta)
{
	KeyInput();
}

void CRect_Model::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CRect_Model::Late_Update(_float fTimeDelta)
{	
	FireSword();

	__super::Late_Update(fTimeDelta);

	if (m_isTPS)
		m_RederID = 2;
	else
		m_RederID = 4;

}

HRESULT CRect_Model::Render()
{
	if (m_isTPS)
	{
		Matrix mat = m_pSteve->GetSoketMatrix(7);
		Matrix mat2 = {};
		mat2.Turn_Radian(_float3(1.f, 0.f, 1.f), D3DXToRadian(90));
		mat2.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(20));

		mat2.Scaling(0.8f, 0.8f, 0.8f);

		m_pVIBufferComs[0]->SetMatrix(m_TPS_mat * mat2 * mat);
	}

	__super::Render();

	return S_OK;
}

HRESULT CRect_Model::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Rect_Model"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Normal"),
		TEXT("m_pVIBufferCom"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[0]))))
		return E_FAIL;

	__super::Ready_Components();

	return S_OK;
}

HRESULT CRect_Model::Ready_Bone()
{
	Matrix mat = {};
	mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(70));
	mat.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(45));
	mat.Set_State(mat.STATE_POSITION, _float3(1.f, -0.5f, 1.2f));

	BONE bone = { "root", -1, mat, mat, Matrix(), Matrix() };
	BONE bone2 = { "Fx", 0, MAtrixTranslation(0.f, 10.f / 16.f, 0.f), MAtrixTranslation(0.f, 10.f / 16.f, 0.f), Matrix(), Matrix()};

	m_pSkeletalAnimator->Add_Bone(bone);
	m_pSkeletalAnimator->Add_Bone(bone2);

	return S_OK;
}

HRESULT CRect_Model::Ready_Animation()
{
	///*------------------------
	//* Init 
	//----------------------------*/
	Matrix mat = {};

	KEYFREAME Init1 = { 0.f, mat };
	m_pSkeletalAnimator->Add_Animation(INIT, Init1);

	///*------------------------
	//* Swing 애니메이션 
	//----------------------------*/
	Matrix matrix1 = {  };
	matrix1.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(100));
	matrix1.Set_State(matrix1.STATE_POSITION, _float3(-1.5f, 0.2f, -0.3f));

	Matrix matrix2 = { matrix1 };
	matrix2.Set_State(matrix1.STATE_POSITION, _float3(-1.5f, -1.5f, 2.f));

	KEYFREAME Swing1 = { 0.f, mat };
	KEYFREAME Swing2 = { 0.15f, matrix1 };
	KEYFREAME Swing3 = { 0.4f, matrix2 };
	KEYFREAME Swing4 = { 1.f, mat };

	m_pSkeletalAnimator->Add_Animation(SWING, Swing1);
	m_pSkeletalAnimator->Add_Animation(SWING, Swing2);
	m_pSkeletalAnimator->Add_Animation(SWING, Swing3);
	m_pSkeletalAnimator->Add_Animation(SWING, Swing4);

	///*------------------------
	//* WALK 애니메이션 
	//----------------------------*/

	matrix1 = { };
	matrix1.Set_State(matrix1.STATE_POSITION, _float3(-0.04f, -0.04f, -0.04f));

	matrix2 = { };
	matrix2.Set_State(matrix2.STATE_POSITION, _float3(-0.04f, 0.04f, -0.04f));

	KEYFREAME Walk1 = { 0.f, mat };
	KEYFREAME Walk2 = { 0.4f, matrix1 };
	KEYFREAME Walk3 = { 0.6f, matrix2 };
	KEYFREAME Walk4 = { 0.8f, matrix1 };
	KEYFREAME Walk5 = { 1.f, mat };

	m_pSkeletalAnimator->Add_Animation(WALK, Walk1);
	m_pSkeletalAnimator->Add_Animation(WALK, Walk2);
	m_pSkeletalAnimator->Add_Animation(WALK, Walk3);
	m_pSkeletalAnimator->Add_Animation(WALK, Walk4);
	m_pSkeletalAnimator->Add_Animation(WALK, Walk5);

///*------------------------
//* Run 애니메이션 
//----------------------------*/

	Walk1 = { 0.f,    Matrix() };
	Walk2 = { 0.125f, matrix1 };
	Walk3 = { 0.25f,  matrix2 };
	Walk4 = { 0.375f, matrix1 };
	Walk5 = { 0.5f,  Matrix() };

	m_pSkeletalAnimator->Add_Animation(RUN, Walk1 );
	m_pSkeletalAnimator->Add_Animation(RUN, Walk2 );
	m_pSkeletalAnimator->Add_Animation(RUN, Walk3 );
	m_pSkeletalAnimator->Add_Animation(RUN, Walk4 );
	m_pSkeletalAnimator->Add_Animation(RUN, Walk5 );


	///*------------------------
	//* EAT 애니메이션 
	//----------------------------*/


	matrix1 = { };
	matrix1.Set_State(mat.STATE_POSITION, _float3(-0.8f, 0.5f, -0.6f));
	matrix1.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-25));
	matrix1.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(-80));
	matrix1.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-20));

	matrix2 = matrix1;
	matrix2.Set_State(mat.STATE_POSITION, _float3(-1.2f, 0.5f, -0.6f));

	KEYFREAME EAT1 = { 0.f, mat };
	KEYFREAME EAT2 = { 0.2f, matrix1 };
	KEYFREAME EAT3 = { 0.3f, matrix2 };
	KEYFREAME EAT4 = { 0.4f, matrix1 };
	KEYFREAME EAT5 = { 0.5f, matrix2 };
	KEYFREAME EAT6 = { 0.6f, matrix1 };
	KEYFREAME EAT7 = { 0.7f, matrix2 };
	KEYFREAME EAT8 = { 0.8f, matrix1 };
	KEYFREAME EAT9 = { 0.9f, matrix2 };
	KEYFREAME EAT10 = { 1.0f, matrix1 };

	m_pSkeletalAnimator->Add_Animation(EAT, EAT1);
	m_pSkeletalAnimator->Add_Animation(EAT, EAT2); 
	m_pSkeletalAnimator->Add_Animation(EAT, EAT3);
	m_pSkeletalAnimator->Add_Animation(EAT, EAT4);
	m_pSkeletalAnimator->Add_Animation(EAT, EAT5);
	m_pSkeletalAnimator->Add_Animation(EAT, EAT6);
	m_pSkeletalAnimator->Add_Animation(EAT, EAT7);
	m_pSkeletalAnimator->Add_Animation(EAT, EAT8);
	m_pSkeletalAnimator->Add_Animation(EAT, EAT9);
	m_pSkeletalAnimator->Add_Animation(EAT, EAT10);


	///*------------------------
	//* Attack1 애니메이션 ( 근거리 공격 )
	//----------------------------*/
	matrix1 = { };
	matrix1.Set_State(Matrix::STATE_POSITION, _float3(0.2f, 0.7f, 0.5f));

	matrix2 = {};
	matrix2.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(60));
	matrix2.Set_State(Matrix::STATE_POSITION, _float3(-0.6f, 0.5f, -0.5f));

	Matrix matrix3 = { };
	matrix3.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(90));
	matrix3.Set_State(Matrix::STATE_POSITION, _float3(-0.3f, -0.5f, -0.5f));

	KEYFREAME Attack_Near_1 = { 0.f, mat };
	KEYFREAME Attack_Near_2 = { 0.2f, matrix1 };
	KEYFREAME Attack_Near_3 = { 0.4f, matrix2 };
	KEYFREAME Attack_Near_4 = { 0.6f, matrix3 };
	KEYFREAME Attack_Near_5 = { 0.8f, mat };

	m_pSkeletalAnimator->Add_Animation(ATTACK_Near, Attack_Near_1);
	m_pSkeletalAnimator->Add_Animation(ATTACK_Near, Attack_Near_2);
	m_pSkeletalAnimator->Add_Animation(ATTACK_Near, Attack_Near_3);
	m_pSkeletalAnimator->Add_Animation(ATTACK_Near, Attack_Near_4);
	m_pSkeletalAnimator->Add_Animation(ATTACK_Near, Attack_Near_5);

	///*------------------------
	//* Attack2 애니메이션 (원거리 참격)
	//----------------------------*/

	matrix1 = { };
	matrix1.Set_State(Matrix::STATE_POSITION, _float3(-0.3f, 0.3f, -1.5f));
	matrix1.Turn_Radian(_float3(-0.3f, -1.f, 0.8f), D3DXToRadian(120));

	matrix2 = {};
	matrix2.Set_State(Matrix::STATE_POSITION, _float3(-0.2f, 0.3f, -0.6f));
	matrix2.Turn_Radian(_float3(0.f, -1.f, 0.f), D3DXToRadian(50));
	matrix2.Turn_Radian(_float3(0.3f, 0.f, 0.3f), D3DXToRadian(60));

	KEYFREAME Attack_Far_1 = { 0.f, mat };
	KEYFREAME Attack_Far_2 = { 0.1f, matrix1 };
	KEYFREAME Attack_Far_3 = { 0.3f, matrix2 };
	KEYFREAME Attack_Far_5 = { 0.5f, mat };

	m_pSkeletalAnimator->Add_Animation(ATTACK_Far, Attack_Far_1);
	m_pSkeletalAnimator->Add_Animation(ATTACK_Far, Attack_Far_2);
	m_pSkeletalAnimator->Add_Animation(ATTACK_Far, Attack_Far_3);
	m_pSkeletalAnimator->Add_Animation(ATTACK_Far, Attack_Far_5);


	return S_OK;
}


void CRect_Model::Update_State(_float fTimeDelta)
{
	switch (m_eCurAnim)
	{
	case INIT:
		Motion_Idle(fTimeDelta);
		break;
	case SWING:
		Motion_Swing(fTimeDelta);
		break;
	case WALK:
		Motion_Walk(fTimeDelta);
		break;
	case EAT:
		Motion_EAT(fTimeDelta);
		break;
	case RUN:
		Motion_Run(fTimeDelta);
		break;
	case ATTACK_Near:
		Motion_Attack1(fTimeDelta);
		break;
	case ATTACK_Far:
		Motion_Attack2(fTimeDelta);
		break;
	case ANIM_END:
		break;
	default:
		break;
	}

}

void CRect_Model::Motion_Idle(_float fTimeDelta)
{
	m_pSkeletalAnimator->Update_Animetion(INIT, fTimeDelta, 0);

	if (m_pSkeletalAnimator->is_AnimtionEND(INIT))
	{
		m_eCurAnim = INIT;
	}
}

void CRect_Model::Motion_Swing(_float fTimeDelta)
{
	m_pSkeletalAnimator->Update_Animetion(SWING, fTimeDelta, 0);	

	
	if (m_pSkeletalAnimator->is_AnimtionEND(SWING))
	{
		if (m_pSteve->Get_AttackContinue())
			m_eCurAnim = SWING;
		else
			m_eCurAnim = INIT;
	}
}

void CRect_Model::Motion_Walk(_float fTimeDelta)
{
	m_pSkeletalAnimator->Update_Animetion(WALK, fTimeDelta, 0);

	if (m_pSkeletalAnimator->is_AnimtionEND(WALK))
	{
		m_eCurAnim = WALK;
	}
}


void CRect_Model::Motion_Run(_float fTimeDelta)
{
	m_pSkeletalAnimator->Update_Animetion(RUN, fTimeDelta, 0);

	if (m_pSkeletalAnimator->is_AnimtionEND(RUN))
	{
		m_eCurAnim = RUN;
	}
}

void CRect_Model::Motion_EAT(_float fTimeDelta)
{	
	ITEMNAME name = ITEMNAME(m_TextrueNum + 100);
	if (name != ITEMNAME_APPLE)
	{
		return;
	}
		
	m_pSkeletalAnimator->Update_Animetion(EAT, fTimeDelta, 0);

	// 먹는 파티클.
	if (m_fCurrentEatTime >= m_fEatCoolTime)
	{
		// 본 행렬.
		Matrix boneWorldMatrix = m_pSkeletalAnimator->GetBoneWorldMatrix(1);

		// 먹는 파티클 작동.	
		CParticleSystem* particle = CParticleEventManager::Get_Instance()->OnParticle(
			PROTOTYPE_GAMEOBJECT_PARTICLE_EATING,
			boneWorldMatrix.Get_State(boneWorldMatrix.STATE_POSITION)
		);

		// 포지션만 넣으면 회전 어긋나니까 이것도 똑같이해.
		particle->GetTransform()->Set_Matrix(boneWorldMatrix);

		// 시간 초기화.
		m_fCurrentEatTime = 0.f;
	}	

	// 먹는 쿨타임.
	m_fCurrentEatTime += fTimeDelta;

	if (m_pSkeletalAnimator->is_AnimtionEND(EAT))
	{
		m_fCurrentEatTime = 0.f;
		m_eCurAnim = INIT;
	}
}

void CRect_Model::Motion_Attack1(_float fTimeDelta)
{
	if (m_pSkeletalAnimator->is_AnimtionEND(ATTACK_Near))
	{
		m_eCurAnim = INIT;
		
	}

	SwingFireSword();
	m_pSkeletalAnimator->Update_Animetion(ATTACK_Near, fTimeDelta, 0);
}

void CRect_Model::Motion_Attack2(_float fTimeDelta)
{
	m_pSkeletalAnimator->Update_Animetion(ATTACK_Far, fTimeDelta, 0);

	if (m_pSkeletalAnimator->is_AnimtionEND(ATTACK_Far))
	{
		m_eCurAnim = INIT;
	}
}

void CRect_Model::KeyInput()
{
	if (m_pGameInstance->Key_Down(VK_LBUTTON))
	{
		ITEMNAME name = ITEMNAME(m_TextrueNum + 100);
		if (name == ITEM_WEPON_1)
		{
			m_eCurAnim = ATTACK_Near;
		}
		else
		{
			m_eCurAnim = SWING;
		}

		return;
	}

	if (m_pGameInstance->Key_Down(VK_RBUTTON))
	{
		if (Compute_Texture_Name() == ITEM_WEPON_1)
		{
			AuraSword();
			m_eCurAnim = ATTACK_Far;
		}
		else
		{
			m_eCurAnim = EAT;
		}
   
		return;
	}

	if (m_eCurAnim == SWING || m_eCurAnim == EAT || m_eCurAnim == ATTACK_Near ||  m_eCurAnim == ATTACK_Far)
		return;

	// 애니메이션 바꾸기
	if (m_pGameInstance->Key_Pressing('W') ||
		m_pGameInstance->Key_Pressing('A') ||
		m_pGameInstance->Key_Pressing('S') ||
		m_pGameInstance->Key_Pressing('D'))
	{
		m_eCurAnim = WALK;
	}
	else
	{
		m_eCurAnim = INIT;
	}
	
}

void CRect_Model::FireSword()
{
	
	if (!m_isRender)
	{
		if (flameSword != nullptr)
		{
			m_pGameInstance->PopPool(
				flameSword,
				PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME
			);

			// 메모리 해제는 안함.
			flameSword = nullptr;
		}		

		return;
	}
	if (Compute_Texture_Name() != ITEM_WEPON_1)
	{
		if (flameSword != nullptr)
		{
			m_pGameInstance->PopPool(
				flameSword,
				PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME
			);

			// 메모리 해제는 안함.
			flameSword = nullptr;
		}

		return;
	}

	// 회전 행렬.
	Matrix rotateMatrix = {};

	// 본 월드행렬.
	Matrix boneWorldMatrix = m_pSkeletalAnimator->GetBoneWorldMatrix(1);

	// 회전행렬 계산.
	rotateMatrix = rotateMatrix.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-35.f));

	if (flameSword == nullptr)
	{
		flameSword = (CParticleSystem*)m_pGameInstance->PushPool(
			LEVEL_STATIC,
			PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME,
			LEVEL_STATIC,
			LAYER_PARTICLE
		);
	}

	// 파티클 적용.
	flameSword->Replay(boneWorldMatrix.Get_State(boneWorldMatrix.STATE_POSITION));

	// 자전 * 부모
	flameSword->GetTransform()->Set_Matrix(rotateMatrix * boneWorldMatrix);
}

void CRect_Model::AuraSword()
{
	// 검기.
	CSwordAura* swordAura = (CSwordAura*)m_pGameInstance->PushPool(
		LEVEL_YU,	// 적용 씬.
		PROTOTYPE_GAMEOBJECT_SWORD_AURA,	// 가져올 프로토타입.
		LEVEL_YU,		// 가져올 씬.
		LAYER_EFFECT	// 애드오브젝트에 추가할 레이어.
	);

	// 카메라.
	CCamera_Player* camera = (CCamera_Player*)m_pGameInstance->Get_Object(
		LEVEL_YU,
		TEXT("Layer_Camera"),
		0
	);

	// 카메라 월드 행렬.
	const _float4x4* cameraWorldmat = camera->GetTransform()->Get_WorldMatrix();

	// 회전 행렬.
	Matrix rotateMatrix = {};
	Matrix scaleMatrix = {};

	// 검기 90도 회전.
	rotateMatrix = rotateMatrix.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f));

	// 검기 크기 늘림.
	scaleMatrix = scaleMatrix.Scaling(5.f, 5.f, 5.f);

	swordAura->GetTransform()->Set_Matrix((_float4x4)scaleMatrix * (_float4x4)rotateMatrix * (*cameraWorldmat));

	// 기존 생성 위치가 좀 낮게.
	_float3 pos = swordAura->GetTransform()->Get_State(CTransform::STATE_POSITION);

	swordAura->GetTransform()->Set_State(
		CTransform::STATE_POSITION,
		{ pos.x, pos.y - 0.3f, pos.z}
		);
}

void CRect_Model::SwingFireSword()
{
	if (Compute_Texture_Name() != ITEM_WEPON_1)
	{
		return;
	}

	// 본 행렬.
	Matrix boneWorldMatrix = m_pSkeletalAnimator->GetBoneWorldMatrix(1);

	CParticleSystem* particle = CParticleEventManager::Get_Instance()->OnParticle(
		PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME,
		boneWorldMatrix.Get_State(boneWorldMatrix.STATE_POSITION)
	);

	particle->GetTransform()->Set_Matrix(boneWorldMatrix);

	particle->SetTimer(0.3f);
}

ITEMNAME CRect_Model::Compute_Texture_Name()
{
	return ITEMNAME(m_TextrueNum + 100);
}

CRect_Model* CRect_Model::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRect_Model* pInstance = new CRect_Model(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRect_Model");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRect_Model::Clone(void* pArg)
{
	CRect_Model* pInstance = new CRect_Model(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRect_Model::Free()
{
	__super::Free();
}
