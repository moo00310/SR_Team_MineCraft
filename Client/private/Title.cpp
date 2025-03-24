#include "Title.h"

CTitle::CTitle(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CTitle::CTitle(CTitle& Prototype)
    : CUIObject( Prototype )
{
}

HRESULT CTitle::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTitle::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc{};

    Desc.fSizeX = 600;
    Desc.fSizeY = 100;
    Desc.fX = g_iWinSizeX * 0.5f;
    Desc.fY = g_iWinSizeY * 0.2f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CTitle::Priority_Update(_float fTimeDelta)
{
}

void CTitle::Update(_float fTimeDelta)
{
}

void CTitle::Late_Update(_float fTimeDelta)
{
    
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CTitle::Render()
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

HRESULT CTitle::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_title"), TEXT("Com_Texture"),
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

CTitle* CTitle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CTitle* pInstance = new CTitle(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CTitle");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CTitle::Clone(void* pArg)
{
    CTitle* pInstance = new CTitle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CTitle");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CTitle::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
