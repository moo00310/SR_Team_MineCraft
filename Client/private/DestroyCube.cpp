#include "DestroyCube.h"

CDestroyCube::CDestroyCube(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CCube(pGraphic_Device)
{

}

CDestroyCube::CDestroyCube(const CDestroyCube& Prototype) :
	CCube(Prototype)
{
}

HRESULT CDestroyCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDestroyCube::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDestroyCube::Priority_Update(_float fTimeDelta)
{
}

void CDestroyCube::Update(_float fTimeDelta)
{
}

void CDestroyCube::Late_Update(_float fTimeDelta)
{
}

HRESULT CDestroyCube::Render()
{
	return S_OK;
}

HRESULT CDestroyCube::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_YU, PROTOTYPE_COMPONENT_VIBUFFER_ONLY,
	    TEXT("m_pVIBufferOnlyCom"), reinterpret_cast<CComponent**>(&m_pVIBufferOnlyCom))))
	    return E_FAIL;

	/* For.Com_Texture */	
	if (FAILED(__super::Add_Component(LEVEL_YU, PROTOTYPE_COMPONENT_TEXTURE_DESTROY,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CDestroyCube* CDestroyCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDestroyCube* pInstance = new CDestroyCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDestroyCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDestroyCube::Clone(void* pArg)
{
	CDestroyCube* pInstance = new CDestroyCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CDestroyCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDestroyCube::Free()
{
	__super::Free();
}
