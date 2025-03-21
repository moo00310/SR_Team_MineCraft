#include "Wood.h"

CWood::CWood(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBreakableCube(pGraphic_Device)
{
}

CWood::CWood(const CWood& Prototype)
	: CBreakableCube(Prototype)
{
}

HRESULT CWood::Initialize_Prototype()
{
	return S_OK;;
}

HRESULT CWood::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CWood::Priority_Update(_float fTimeDelta)
{
}

void CWood::Update(_float fTimeDelta)
{
}

void CWood::Late_Update(_float fTimeDelta)
{

}

HRESULT CWood::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;


}

HRESULT CWood::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Wood"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("m_pVIBufferCom_Cube"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CWood* CWood::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWood* pInstance = new CWood(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWood");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWood::Clone(void* pArg)
{
	CWood* pInstance = new CWood(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CWood");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWood::Free()
{
	__super::Free();
}

