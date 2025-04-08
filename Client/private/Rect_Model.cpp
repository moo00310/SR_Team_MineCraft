#include "Rect_Model.h"

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

	flameSword = (CParticleSystem*)m_pGameInstance->PushPool(
		LEVEL_STATIC,
		PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME,
		LEVEL_STATIC,
		LAYER_PARTICLE
		);	

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
	BONE bone2 = { "Fx", 0, MAtrixTranslation(4.f/16.f, 4.f / 16.f, 0.f), MAtrixTranslation(4.f / 16.f, 4.f / 16.f, 0.f), Matrix(), Matrix()};

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
	//* Swing �ִϸ��̼� 
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
	//* WALK �ִϸ��̼� 
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
	//* EAT �ִϸ��̼� 
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

	SwingFireSword();

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

void CRect_Model::Motion_EAT(_float fTimeDelta)
{	
	ITEMNAME name = ITEMNAME(m_TextrueNum + 100);
	if (name != ITEMNAME_APPLE)
	{
		return;
	}
		
	m_pSkeletalAnimator->Update_Animetion(EAT, fTimeDelta, 0);

	// �Դ� ��ƼŬ.
	if (m_fCurrentEatTime >= m_fEatCoolTime)
	{
		// �� ���.
		Matrix boneWorldMatrix = m_pSkeletalAnimator->GetBoneWorldMatrix(1);

		// �Դ� ��ƼŬ �۵�.	
		CParticleSystem* particle = CParticleEventManager::Get_Instance()->OnParticle(
			PROTOTYPE_GAMEOBJECT_PARTICLE_EATING,
			boneWorldMatrix.Get_State(boneWorldMatrix.STATE_POSITION)
		);

		// �����Ǹ� ������ ȸ�� ��߳��ϱ� �̰͵� �Ȱ�����.
		particle->GetTransform()->Set_Matrix(boneWorldMatrix);

		// �ð� �ʱ�ȭ.
		m_fCurrentEatTime = 0.f;
	}	

	// �Դ� ��Ÿ��.
	m_fCurrentEatTime += fTimeDelta;

	if (m_pSkeletalAnimator->is_AnimtionEND(EAT))
	{
		m_fCurrentEatTime = 0.f;
		m_eCurAnim = INIT;
	}
}

void CRect_Model::KeyInput()
{
	if (m_pGameInstance->Key_Down(VK_LBUTTON))
	{
		m_eCurAnim = SWING;
		return;
	}

	if (m_pGameInstance->Key_Down(VK_RBUTTON))
	{
		m_eCurAnim = EAT;		
		//AuraSword();
		SonicBoom();
		return;
	}

	if (m_eCurAnim == SWING || m_eCurAnim == EAT)
		return;

	// �ִϸ��̼� �ٲٱ�
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
		return;
	}
	if (Compute_Texture_Name() == ITEM_WEPON_1)
	{
		// ȸ�� ���.
		Matrix rotateMatrix = {};

		// �� �������.
		Matrix boneWorldMatrix = m_pSkeletalAnimator->GetBoneWorldMatrix(1);

		// ȸ����� ���.
		rotateMatrix = rotateMatrix.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-35.f));

		// ��ƼŬ ����.
		flameSword->Replay(boneWorldMatrix.Get_State(boneWorldMatrix.STATE_POSITION));

		// ���� * �θ�
		flameSword->GetTransform()->Set_Matrix(rotateMatrix * boneWorldMatrix);
	}
	else
	{
		flameSword->Replay(_float3(0.f,0.f,0.f));
	}

	
}

void CRect_Model::AuraSword()
{
	// �˱�.
	CSwordAura* swordAura = (CSwordAura*)m_pGameInstance->PushPool(
		LEVEL_YU,	// ���� ��.
		PROTOTYPE_GAMEOBJECT_SWORD_AURA,	// ������ ������Ÿ��.
		LEVEL_YU,		// ������ ��.
		LAYER_EFFECT	// �ֵ������Ʈ�� �߰��� ���̾�.
	);

	// ī�޶�.
	CCamera_Player* camera = (CCamera_Player*)m_pGameInstance->Get_Object(
		LEVEL_YU,
		TEXT("Layer_Camera"),
		0
	);

	// ī�޶� ���� ���.
	const _float4x4* cameraWorldmat = camera->GetTransform()->Get_WorldMatrix();

	// ȸ�� ���.
	Matrix rotateMatrix = {};
	Matrix scaleMatrix = {};

	// �˱� 90�� ȸ��.
	rotateMatrix = rotateMatrix.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-90.f));

	// �˱� ũ�� �ø�.
	scaleMatrix = scaleMatrix.Scaling(5.f, 5.f, 5.f);

	swordAura->GetTransform()->Set_Matrix((_float4x4)scaleMatrix * (_float4x4)rotateMatrix * (*cameraWorldmat));

	// ���� ���� ��ġ�� �� ����.
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

	// �� ���.
	Matrix boneWorldMatrix = m_pSkeletalAnimator->GetBoneWorldMatrix(1);

	CParticleSystem* particle = CParticleEventManager::Get_Instance()->OnParticle(
		PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME,
		boneWorldMatrix.Get_State(boneWorldMatrix.STATE_POSITION)
	);

	particle->GetTransform()->Set_Matrix(boneWorldMatrix);

	//particle->SetTimer(0.3f);
}

void CRect_Model::SonicBoom()
{
	// ī�޶�.
	CCamera_Player* camera = (CCamera_Player*)m_pGameInstance->Get_Object(
		LEVEL_YU,
		TEXT("Layer_Camera"),
		0
	);

	// ī�޶� ���� ���.
	const _float4x4* cameraWorldmat = camera->GetTransform()->Get_WorldMatrix();

	CParticleEventManager::Get_Instance()->OnParticle(
		PROTOTYPE_GAMEOBJECT_PARTICLE_SONIC_BOOM,
		{0.f, 0.f, 0.f}
	)->GetTransform()->Set_Matrix(*cameraWorldmat);
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
