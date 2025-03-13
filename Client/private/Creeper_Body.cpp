#include "Creeper_Body.h"

#include "GameInstance.h"

CCreeper_Body::CCreeper_Body(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CCreeper_Body::CCreeper_Body(const CCreeper_Body& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CCreeper_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCreeper_Body::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCreeper_Body::Priority_Update(_float fTimeDelta)
{
}

void CCreeper_Body::Update(_float fTimeDelta)
{

}

void CCreeper_Body::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
		return;
}

HRESULT CCreeper_Body::Render()
{
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

HRESULT CCreeper_Body::Ready_Components()
{
	// 크리퍼 텍스처
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_MOO, TEXT("Prototype_Component_Texture_Creeper"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_MOO, TEXT("Prototype_Component_VIBuffer_Creeper_Body"),
		TEXT("m_pVIBufferCom_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CCreeper_Body* CCreeper_Body::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCreeper_Body* pInstance = new CCreeper_Body(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCreeper_Body");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCreeper_Body::Clone(void* pArg)
{
	CCreeper_Body* pInstance = new CCreeper_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCreeper_Body");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCreeper_Body::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
