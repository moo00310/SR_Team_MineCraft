#include "Grass.h"
#include "GameInstance.h"

CGrass::CGrass(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableRect(pGraphic_Device)
{
}

CGrass::CGrass(const CGrass& Prototype)
    : CBreakableRect(Prototype)
{
}

HRESULT CGrass::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGrass::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;
    return S_OK;
}

void CGrass::Priority_Update(_float fTimeDelta)
{
}

void CGrass::Update(_float fTimeDelta)
{
}

void CGrass::Late_Update(_float fTimeDelta)
{
    if (m_bRenderActive) {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }

	__super::Late_Update(fTimeDelta);
}

HRESULT CGrass::Render()
{
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

    return S_OK;
}

HRESULT CGrass::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Grass"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("m_pVIBufferCom_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

	__super::Ready_Components();

    return S_OK;
}


CGrass* CGrass::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CGrass* pInstance = new CGrass(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CGrass");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGrass::Clone(void* pArg)
{
    CGrass* pInstance = new CGrass(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CGrass");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGrass::Free()
{
    __super::Free();
}
