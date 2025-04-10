#include "MainInventory.h"
#include "UI_Mgr.h"

_bool g_bMainInventoryOpen = false;

CMainInventory::CMainInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CMainInventory::CMainInventory(CMainInventory& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CMainInventory::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMainInventory::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc{};

    Desc.fSizeX = 550.f;
    Desc.fSizeY = 500.f;
    Desc.fX = g_iWinSizeX * 0.5f;
    Desc.fY = 300.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CMainInventory::Priority_Update(_float fTimeDelta)
{
}

void CMainInventory::Update(_float fTimeDelta)
{
}

void CMainInventory::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CMainInventory::Render()
{
    if (g_bMainInventoryOpen)
    {
        if (FAILED(m_pTextureCom->Bind_Resource(1)))
            return E_FAIL;

        if (FAILED(m_pVIBufferCom->Bind_Buffers()))
            return E_FAIL;

        if (FAILED(m_pTransformCom->Bind_Resource()))
            return E_FAIL;

        __super::Begin();

        if (FAILED(m_pVIBufferCom->Render()))
            return E_FAIL;

        __super::End();
    }

    return S_OK;
}

HRESULT CMainInventory::Ready_Components()
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

CMainInventory* CMainInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMainInventory* pInstance = new CMainInventory(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMainInventory");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CMainInventory::Clone(void* pArg)
{
    CMainInventory* pInstance = new CMainInventory(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CMainInventory");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CMainInventory::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
