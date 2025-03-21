#include "Edition.h"

CEdition::CEdition(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CEdition::CEdition(CEdition& Prototype)
	: CUIObject( Prototype )
{
}

HRESULT CEdition::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEdition::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc{};
    
    //Desc.fSizeX = 100;
    //Desc.fSizeY = 100;
    Desc.fSizeX = 300;
    Desc.fSizeY = 50;
    Desc.fX = g_iWinSizeX * 0.5f;
    Desc.fY = g_iWinSizeY * 0.25f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CEdition::Priority_Update(_float fTimeDelta)
{
}

void CEdition::Update(_float fTimeDelta)
{
}

void CEdition::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CEdition::Render()
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

HRESULT CEdition::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_WOO, TEXT("Prototype_Component_Texture_edition"), TEXT("Com_Texture"),
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

CEdition* CEdition::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CEdition* pInstance = new CEdition(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CEdition");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CEdition::Clone(void* pArg)
{
    CEdition* pInstance = new CEdition(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CEdition");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CEdition::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
