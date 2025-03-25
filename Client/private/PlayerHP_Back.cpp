#include "PlayerHP_Back.h"

CPlayerHP_Back::CPlayerHP_Back(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CPlayerHP_Back::CPlayerHP_Back(CPlayerHP_Back& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CPlayerHP_Back::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerHP_Back::Initialize(void* pArg)
{
    m_iHpIndex = (int*)pArg;
    m_iHpCount = *m_iHpIndex;

    Desc.fSizeX = 35.f;
    Desc.fSizeY = 35.f;
    Desc.fX = 350.f + (m_iHpCount * 35.f);
    Desc.fY = g_iWinSizeY * 0.85f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CPlayerHP_Back::Priority_Update(_float fTimeDelta)
{
}

void CPlayerHP_Back::Update(_float fTimeDelta)
{
}

void CPlayerHP_Back::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CPlayerHP_Back::Render()
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

HRESULT CPlayerHP_Back::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_PlayerHP"), TEXT("Com_Texture"),
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

void CPlayerHP_Back::Begin()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 160);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CPlayerHP_Back::End()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CPlayerHP_Back* CPlayerHP_Back::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayerHP_Back* pInstance = new CPlayerHP_Back(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPlayerHP_Back");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CPlayerHP_Back::Clone(void* pArg)
{
    CPlayerHP_Back* pInstance = new CPlayerHP_Back(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CPlayerHP_Back");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CPlayerHP_Back::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
