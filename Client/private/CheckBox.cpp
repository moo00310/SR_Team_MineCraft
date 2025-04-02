#include "CheckBox.h"
#include "UI_Mgr.h"

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
    Desc.fSizeX = 85.f;
    Desc.fSizeY = 100.f;
    
    Desc.fX = 360.f;
    Desc.fY = 672.f;

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
    if (m_pGameInstance->Key_Down('1')) { Desc.fX = 360.f; m_iCheckBoxIndex = 0;}
    if (m_pGameInstance->Key_Down('2')) { Desc.fX = 430.f; m_iCheckBoxIndex = 1;}
    if (m_pGameInstance->Key_Down('3')) { Desc.fX = 500.f; m_iCheckBoxIndex = 2;}
    if (m_pGameInstance->Key_Down('4')) { Desc.fX = 570.f; m_iCheckBoxIndex = 3;}
    if (m_pGameInstance->Key_Down('5')) { Desc.fX = 640.f; m_iCheckBoxIndex = 4;}
    if (m_pGameInstance->Key_Down('6')) { Desc.fX = 710.f; m_iCheckBoxIndex = 5;}
    if (m_pGameInstance->Key_Down('7')) { Desc.fX = 780.f; m_iCheckBoxIndex = 6;}
    if (m_pGameInstance->Key_Down('8')) { Desc.fX = 850.f; m_iCheckBoxIndex = 7;}
    if (m_pGameInstance->Key_Down('9')) { Desc.fX = 920.f; m_iCheckBoxIndex = 8;}

    if (m_fCheckBox != Desc.fX)
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
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_CheckBox"), TEXT("Com_Texture"),
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

    CUI_Mgr::Get_Instance()->Get_vecCheckBoxlist()->push_back(pInstance);

    return pInstance;
}

void CCheckBox::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
