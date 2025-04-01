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
	m_TextrueNum = 0;

	__super::Initialize(pArg);
	return S_OK;
}

void CRect_Model::Priority_Update(_float fTimeDelta)
{

}

void CRect_Model::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CRect_Model::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CRect_Model::Render()
{
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
	// 스윙 프레임 행렬을 벡터에 저장
	Matrix mat = {};
	mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(70));
	mat.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(45));
	mat.Set_State(mat.STATE_POSITION, _float3(1.f, -0.5f, 1.2f));

	BONE bone = { "root", -1, mat, mat, Matrix(), Matrix() };

	m_pSkeletalAnimator->Add_Bone(bone);

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
	Matrix matrix1 = {};
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


	///*------------------------
	//* EAT 애니메이션 
	//----------------------------*/

	return S_OK;
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
		m_eCurAnim = INIT;
		isAttack = false;
	}
}

void CRect_Model::Motion_Walk(_float fTimeDelta)
{
	/*m_pSkeletalAnimator->Update_Animetion(WALK, fTimeDelta, 0);

	if (m_pSkeletalAnimator->is_AnimtionEND(WALK))
	{
		m_eCurAnim = WALK;
	}*/
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
		MSG_BOX("Failed to Created : CRect_Model");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRect_Model::Free()
{
	__super::Free();
}
