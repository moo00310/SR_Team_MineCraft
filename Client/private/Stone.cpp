#include "Stone.h"
#include "GameInstance.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CStone::CStone(const CStone& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CStone::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStone::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CStone::Priority_Update(_float fTimeDelta)
{
}

void CStone::Update(_float fTimeDelta)
{
}

void CStone::Late_Update(_float fTimeDelta)
{
    if (m_bRenderActive) {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
            return;
    }
}

HRESULT CStone::Render()
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

HRESULT CStone::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Stone"),
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


CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CStone* pInstance = new CStone(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CStone");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CStone::Clone(void* pArg)
{
    CStone* pInstance = new CStone(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CStone");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CStone::Free()
{
    __super::Free();
}
