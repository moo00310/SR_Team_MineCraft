#include "PlayerHunger_Back.h"

CPlayerHunger_Back::CPlayerHunger_Back(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CPlayerHunger_Back::CPlayerHunger_Back(CPlayerHunger_Back& Prototype)
    : CUIObject( Prototype )
{
}

HRESULT CPlayerHunger_Back::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerHunger_Back::Initialize(void* pArg)
{
    m_iHungerCount = (int*)pArg;
    m_iHungerIndex = *m_iHungerCount;

    Desc.fSizeX = 30.f;
    Desc.fSizeY = 30.f;
    Desc.fX = 680.f + (m_iHungerIndex * Desc.fSizeX);
    Desc.fY = 570.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CPlayerHunger_Back::Priority_Update(_float fTimeDelta)
{
}

void CPlayerHunger_Back::Update(_float fTimeDelta)
{
}

void CPlayerHunger_Back::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CPlayerHunger_Back::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    __super::Begin();
    Begin();

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    __super::End();
    End();

    return S_OK;
}

HRESULT CPlayerHunger_Back::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_PlayerHunger"), TEXT("Com_Texture"),
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

void CPlayerHunger_Back::Begin()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CPlayerHunger_Back::End()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CPlayerHunger_Back* CPlayerHunger_Back::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayerHunger_Back* pInstance = new CPlayerHunger_Back(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPlayerHunger_Back");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CPlayerHunger_Back::Clone(void* pArg)
{
    CPlayerHunger_Back* pInstance = new CPlayerHunger_Back(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CPlayerHunger_Back");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CPlayerHunger_Back::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
