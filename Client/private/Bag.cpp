#include "Bag.h"
#include "UI_Mgr.h"

CBag::CBag(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUIObject{ pGraphic_Device }
{
}

CBag::CBag(CBag& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CBag::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBag::Initialize(void* pArg)
{
    
    Desc.fSizeX = 64.f;
    Desc.fSizeY = 64.f;
    Desc.fX = 280.f;
    Desc.fY = 672.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CBag::Priority_Update(_float fTimeDelta)
{
}

void CBag::Update(_float fTimeDelta)
{
}

void CBag::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CBag::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(2)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    __super::Begin();
    SetUp_RenderState();

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    __super::End();
    Reset_RenderState();

    return S_OK;
}

CTransform* CBag::GetTransform() const
{
    return m_pTransformCom;
}

HRESULT CBag::Ready_Components()
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

HRESULT CBag::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CBag::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    return S_OK;
}

CBag* CBag::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBag* pInstance = new CBag(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBag");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CBag::Clone(void* pArg)
{
    CBag* pInstance = new CBag(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CBag");
        Safe_Release(pInstance);
    }

    CUI_Mgr::Get_Instance()->Set_Bag(pInstance);

    return pInstance;
}

void CBag::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
