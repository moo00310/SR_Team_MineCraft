#include "IronOre.h"
#include "GameInstance.h"

CIronOre::CIronOre(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CIronOre::CIronOre(const CIronOre& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CIronOre::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CIronOre::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CIronOre::Priority_Update(_float fTimeDelta)
{
}

void CIronOre::Update(_float fTimeDelta)
{
}

void CIronOre::Late_Update(_float fTimeDelta)
{
    if (m_bRenderActive) {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
            return;
    }
}

HRESULT CIronOre::Render()
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

HRESULT CIronOre::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_IronOre"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_BreakableCube"),
        TEXT("m_pVIBufferCom_BreakalbeCube"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    return S_OK;
}


CIronOre* CIronOre::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CIronOre* pInstance = new CIronOre(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CIronOre");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CIronOre::Clone(void* pArg)
{
    CIronOre* pInstance = new CIronOre(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CIronOre");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIronOre::Free()
{
    __super::Free();
}
