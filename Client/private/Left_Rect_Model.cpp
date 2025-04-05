#include "Left_Rect_Model.h"

CLeft_Rect_Model::CLeft_Rect_Model(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CRightHand_Object{ pGraphic_Device }
{
}

CLeft_Rect_Model::CLeft_Rect_Model(const CLeft_Rect_Model& Prototype)
	: CRightHand_Object(Prototype)
{
}

HRESULT CLeft_Rect_Model::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLeft_Rect_Model::Initialize(void* pArg)
{
	m_RederID = 2;

	__super::Initialize(pArg);
	return S_OK;
}

void CLeft_Rect_Model::Priority_Update(_float fTimeDelta)
{
	KeyInput();
}

void CLeft_Rect_Model::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CLeft_Rect_Model::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	if (m_isTPS)
		m_RederID = 2;
	else
		m_RederID = 4;

}

HRESULT CLeft_Rect_Model::Render()
{
	if (m_isTPS)
	{
		Matrix mat = m_pSteve->GetSoketMatrix(8);
		Matrix mat2 = {};
		mat2.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(110));
		mat2.Scaling(0.8f, 0.8f, 0.8f);

		m_pVIBufferComs[0]->SetMatrix(m_TPS_mat * mat2 * mat);
	}
	__super::Render();

	return S_OK;
}

HRESULT CLeft_Rect_Model::Ready_Components()
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

HRESULT CLeft_Rect_Model::Ready_Bone()
{
	Matrix mat = {};
	mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(-70));
	mat.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(45));
	mat.Set_State(mat.STATE_POSITION, _float3(-1.f, -0.5f, 1.2f));

	BONE bone = { "root", -1, mat, mat, Matrix(), Matrix() };

	m_pSkeletalAnimator->Add_Bone(bone);

	return S_OK;
}

HRESULT CLeft_Rect_Model::Ready_Animation()
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



	return S_OK;
}

void CLeft_Rect_Model::Update_State(_float fTimeDelta)
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
	case ANIM_END:
		break;
	default:
		break;
	}
}

void CLeft_Rect_Model::Motion_Idle(_float fTimeDelta)
{
	m_pSkeletalAnimator->Update_Animetion(INIT, fTimeDelta, 0);

	if (m_pSkeletalAnimator->is_AnimtionEND(INIT))
	{
		m_eCurAnim = INIT;
	}
}

void CLeft_Rect_Model::Motion_Swing(_float fTimeDelta)
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

void CLeft_Rect_Model::Motion_Walk(_float fTimeDelta)
{
	m_pSkeletalAnimator->Update_Animetion(WALK, fTimeDelta, 0);

	if (m_pSkeletalAnimator->is_AnimtionEND(WALK))
	{
		m_eCurAnim = WALK;
	}
}

void CLeft_Rect_Model::KeyInput()
{
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

CLeft_Rect_Model* CLeft_Rect_Model::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLeft_Rect_Model* pInstance = new CLeft_Rect_Model(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLeft_Rect_Model");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeft_Rect_Model::Clone(void* pArg)
{
	CGameObject* pInstance = new CLeft_Rect_Model(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeft_Rect_Model::Free()
{
	__super::Free();
}
