#include "MainLogo.h"
#include "GameInstance.h"

CMainLogo::CMainLogo(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUIObject{ pGraphic_Device }
{
}

CMainLogo::CMainLogo(CMainLogo& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CMainLogo::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMainLogo::Initialize(void* pArg)
{
    UIOBJECT_DESC Desc{};

    Desc.fSizeX = g_iWinSizeX + 200.f;
    Desc.fSizeY = g_iWinSizeY;
    Desc.fX = g_iWinSizeX * 0.5f;
    Desc.fY = g_iWinSizeY * 0.5f;
    
    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0, 0, 0.f));
    
    return S_OK;
}

void CMainLogo::Priority_Update(_float fTimeDelta)
{
}

void CMainLogo::Update(_float fTimeDelta)
{
    /* 시간 누적 */
    elapsedTime += fTimeDelta;

    if (5.0f > elapsedTime)
    {
        m_pTransformCom->Go_Left(0.016f);
    }
    if (elapsedTime >= 5.0f)
    {
        m_pTransformCom->Go_Right(0.016f);
    }
    if (elapsedTime >= 10.0f)
    {
        elapsedTime = 0.0f;
    }
}

void CMainLogo::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CMainLogo::Render()
{
   // m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

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


CMainLogo* CMainLogo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMainLogo* pInstance = new CMainLogo(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMainLogo");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CMainLogo::Clone(void* pArg)
{
    CMainLogo* pInstance = new CMainLogo(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CMainLogo");
        Safe_Release(pInstance );
    }
    return pInstance;
}

HRESULT CMainLogo::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_MainLogo"), TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    CTransform::TRANSFORM_DESC TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    return S_OK;
}

void CMainLogo::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
