#include "MissionMainUi.h"

CMissionMainUi::CMissionMainUi(LPDIRECT3DDEVICE9 pGraphic_Device) :CUIObject{ pGraphic_Device }
{
}

CMissionMainUi::CMissionMainUi(CMissionMainUi& Prototype) :CUIObject(Prototype)
{
}

HRESULT CMissionMainUi::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMissionMainUi::Initialize(void* pArg)
{
    Desc.fSizeX = 300.f;
    Desc.fSizeY = 400.f;
    Desc.fX = 500.f;
    Desc.fY = 300.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CMissionMainUi::Priority_Update(_float fTimeDelta)
{
}

void CMissionMainUi::Update(_float fTimeDelta)
{
}

void CMissionMainUi::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CMissionMainUi::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    __super::Begin();
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    m_pShaderCom->Bind_Texture("g_Texture", m_pTextureCom->Get_Texture(0));

    m_pShaderCom->Begin(4);

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pShaderCom->End();
    __super::End();
    return S_OK;
}

HRESULT CMissionMainUi::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_MissionMainUi"), TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

CMissionMainUi* CMissionMainUi::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMissionMainUi* pInstance = new CMissionMainUi(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMissionMainUi");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CMissionMainUi::Clone(void* pArg)
{
    CMissionMainUi* pInstance = new CMissionMainUi(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CMissionMainUi");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CMissionMainUi::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
}
