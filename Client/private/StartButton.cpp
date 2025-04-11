#include "StartButton.h"

CStartButton::CStartButton(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CStartButton::CStartButton(CStartButton& Prototype)
    : CUIObject( Prototype )
{
}

HRESULT CStartButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStartButton::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc{};

    Desc.fSizeX = 650;
    Desc.fSizeY = 100;
    Desc.fX = g_iWinSizeX * 0.5f;
    Desc.fY = g_iWinSizeY * 0.5f + 200;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
    
 	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));


    return S_OK;
}

void CStartButton::Priority_Update(_float fTimeDelta)
{
}

void CStartButton::Update(_float fTimeDelta)
{
    if (m_pGameInstance->Key_Down(VK_LBUTTON)) {
        if (true == __super::isPick(g_hWnd))
            g_bChangeLevel = true;
    }
}

void CStartButton::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CStartButton::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    __super::Begin();

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    __super::End();

    return S_OK;
}

HRESULT CStartButton::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_StartButton"), TEXT("Com_Texture"),
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

CStartButton* CStartButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CStartButton* pInstance = new CStartButton(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CStartButton");
        Safe_Release(pInstance);
    }
    return pInstance;

}

CGameObject* CStartButton::Clone(void* pArg)
{
    CStartButton* pInstance = new CStartButton(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CStartButton");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CStartButton::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
