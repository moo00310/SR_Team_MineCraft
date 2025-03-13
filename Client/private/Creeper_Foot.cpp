#include "Creeper_Foot.h"
#include "GameInstance.h"

CCreeper_Foot::CCreeper_Foot(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject{ pGraphic_Device }
{
}

CCreeper_Foot::CCreeper_Foot(const CCreeper_Foot& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CCreeper_Foot::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCreeper_Foot::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CCreeper_Foot::Priority_Update(_float fTimeDelta)
{
}

void CCreeper_Foot::Update(_float fTimeDelta)
{
}

void CCreeper_Foot::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
        return;
}

HRESULT CCreeper_Foot::Render()
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

HRESULT CCreeper_Foot::Ready_Components()
{
	// 크리퍼 텍스처
	 /* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_MOO, TEXT("Prototype_Component_Texture_Creeper"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_MOO, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
		TEXT("m_pVIBufferCom_Foot"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CCreeper_Foot* CCreeper_Foot::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCreeper_Foot* pInstance = new CCreeper_Foot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCreeper_Foot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCreeper_Foot::Clone(void* pArg)
{
	CCreeper_Foot* pInstance = new CCreeper_Foot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCreeper_Foot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCreeper_Foot::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
