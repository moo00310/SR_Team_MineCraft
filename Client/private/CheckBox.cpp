#include "CheckBox.h"

CCheckBox::CCheckBox(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CCheckBox::CCheckBox(CCheckBox& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CCheckBox::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCheckBox::Initialize(void* pArg)
{
 
    Desc.fSizeX = 77;
    Desc.fSizeY = 77;
    Desc.fX = g_iWinSizeX * 0.293f;
    Desc.fY = g_iWinSizeY * 0.95f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CCheckBox::Priority_Update(_float fTimeDelta)
{
}

void CCheckBox::Update(_float fTimeDelta)
{
    if (GetKeyState('1') & 0x8000)
    {
        Desc.fX = g_iWinSizeX * 0.293f;
    }
    if (GetKeyState('2') & 0x8000)
    {
        Desc.fX = g_iWinSizeX * 0.346f;
    }
    if (GetKeyState('3') & 0x8000)
    {
        Desc.fX = g_iWinSizeX * 0.397f; 
    }
    if (GetKeyState('4') & 0x8000)
    {
        Desc.fX = g_iWinSizeX * 0.450f;
    }
    if (GetKeyState('5') & 0x8000)
    {
        Desc.fX = g_iWinSizeX * 0.502f;
    }
    if (GetKeyState('6') & 0x8000)
    {
        Desc.fX = g_iWinSizeX * 0.554f;
    }
    if (GetKeyState('7') & 0x8000)
    {
        Desc.fX = g_iWinSizeX * 0.604f;
    }
    if (GetKeyState('8') & 0x8000)
    {
        Desc.fX = g_iWinSizeX * 0.655f;
    }
    if (GetKeyState('9') & 0x8000)
    {
        Desc.fX = g_iWinSizeX * 0.707f;
    }
    
    if(m_fCheckBox != Desc.fX)
    {
       m_fCheckBox = Desc.fX;
       __super::Initialize(&Desc);
       m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    }
}

void CCheckBox::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CCheckBox::Render()
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

HRESULT CCheckBox::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_Texture_CheckBox"), TEXT("Com_Texture"),
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

CCheckBox* CCheckBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCheckBox* pInstance = new CCheckBox(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CEdition");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CCheckBox::Clone(void* pArg)
{
    CCheckBox* pInstance = new CCheckBox(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CEdition");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CCheckBox::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
