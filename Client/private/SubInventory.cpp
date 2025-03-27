#include "SubInventory.h"

CSubInventory::CSubInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CSubInventory::CSubInventory(CSubInventory& Prototype)
    : CUIObject( Prototype )
{
}

HRESULT CSubInventory::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSubInventory::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc{};

    Desc.fSizeX = g_iWinSizeX * 0.5f;
    Desc.fSizeY = 100.f;
    Desc.fX = g_iWinSizeX * 0.5f;
    Desc.fY = 672.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CSubInventory::Priority_Update(_float fTimeDelta)
{
}

void CSubInventory::Update(_float fTimeDelta)
{
}

void CSubInventory::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CSubInventory::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    __super::Begin();
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 170);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    __super::End();
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    return S_OK; 
}

HRESULT CSubInventory::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Inventory"), TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CSubInventory* CSubInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSubInventory* pInstance = new CSubInventory(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CSubInventory");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CSubInventory::Clone(void* pArg)
{
    CSubInventory* pInstance = new CSubInventory(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CSubInventory");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CSubInventory::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
