#include "PlayerExp_Back.h"

CPlayerExp_Back::CPlayerExp_Back(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CPlayerExp_Back::CPlayerExp_Back(CPlayerExp_Back& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CPlayerExp_Back::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerExp_Back::Initialize(void* pArg)
{
    m_iExpCount = (int*)pArg;
    m_iExpIndex = *m_iExpCount;

    Desc.fSizeX = 36.f;
    Desc.fSizeY = 25.f;
    Desc.fX = 332.f + (m_iExpIndex * Desc.fSizeX);
    Desc.fY = 600.f;

    if (m_iExpIndex == 0)
    {
        m_iTextureNum = 0;
    }
    else if (m_iExpIndex == 17)
    {
        m_iTextureNum = 2;
    }
    else
    {
        m_iTextureNum = 1;
    }

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CPlayerExp_Back::Priority_Update(_float fTimeDelta)
{
}

void CPlayerExp_Back::Update(_float fTimeDelta)
{
}

void CPlayerExp_Back::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CPlayerExp_Back::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(m_iTextureNum)))
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

HRESULT CPlayerExp_Back::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_PlayerExp"), TEXT("Com_Texture"),
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

void CPlayerExp_Back::Begin()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CPlayerExp_Back::End()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

CPlayerExp_Back* CPlayerExp_Back::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayerExp_Back* pInstance = new CPlayerExp_Back(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPlayerExp_Back");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CPlayerExp_Back::Clone(void* pArg)
{
    CPlayerExp_Back* pInstance = new CPlayerExp_Back(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CPlayerExp_Back");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CPlayerExp_Back::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
