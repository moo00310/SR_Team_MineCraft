#include "CoalOre.h"
#include "GameInstance.h"

CCoalOre::CCoalOre(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CCoalOre::CCoalOre(const CCoalOre& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CCoalOre::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCoalOre::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CCoalOre::Priority_Update(_float fTimeDelta)
{
}

void CCoalOre::Update(_float fTimeDelta)
{
}

void CCoalOre::Late_Update(_float fTimeDelta)
{
    if (m_bRenderActive) {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
            return;
    }
}

HRESULT CCoalOre::Render()
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

HRESULT CCoalOre::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_CoalOre"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_CubeInstance"),
        TEXT("m_pVIBufferCom_CubeInstance"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    return S_OK;
}


CCoalOre* CCoalOre::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCoalOre* pInstance = new CCoalOre(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCoalOre");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCoalOre::Clone(void* pArg)
{
    CCoalOre* pInstance = new CCoalOre(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CCoalOre");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCoalOre::Free()
{
    __super::Free();
}
