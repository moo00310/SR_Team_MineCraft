#include "RedTulip.h"
#include "GameInstance.h"

CRedTulip::CRedTulip(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableRect(pGraphic_Device)
{
}

CRedTulip::CRedTulip(const CRedTulip& Prototype)
    : CBreakableRect(Prototype)
{
}

HRESULT CRedTulip::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRedTulip::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(0.5, 0.7, 0.5);

    return S_OK;
}

void CRedTulip::Priority_Update(_float fTimeDelta)
{
}

void CRedTulip::Update(_float fTimeDelta)
{
}

void CRedTulip::Late_Update(_float fTimeDelta)
{
    if (m_bRenderActive) {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }

	__super::Late_Update(fTimeDelta);
}

HRESULT CRedTulip::Render()
{
    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;
    __super::Render();

    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
    return S_OK;
}

HRESULT CRedTulip::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_RedTulip"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;


	__super::Ready_Components();

    return S_OK;
}


CRedTulip* CRedTulip::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CRedTulip* pInstance = new CRedTulip(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CRedTulip");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CRedTulip::Clone(void* pArg)
{
    CRedTulip* pInstance = new CRedTulip(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CRedTulip");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRedTulip::Free()
{
    __super::Free();
}
