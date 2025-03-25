#include "ItemWood.h"
#include "GameInstance.h"

CItemWood::CItemWood(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CItemCube(pGraphic_Device)
{
}

CItemWood::CItemWood(const CItemWood& Prototype)
    : CItemCube(Prototype)
{
}

HRESULT CItemWood::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItemWood::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_fUpDownSpeed = 0.005f;
    m_iUpDownFrame = 0;

    m_pTransformCom->Scaling(0.3, 0.3, 0.3);
    return S_OK;
}

void CItemWood::Priority_Update(_float fTimeDelta)
{
    m_iUpDownFrame++;
    if (m_iUpDownFrame > 20) {
        m_iUpDownFrame = 0;
        m_fUpDownSpeed *= -1;
    }
    m_pTransformCom->Turn(_float3(0, 1, 0), fTimeDelta);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(0, m_fUpDownSpeed, 0));
    

}

void CItemWood::Update(_float fTimeDelta)
{
}

void CItemWood::Late_Update(_float fTimeDelta)
{
    if (m_bRenderActive) {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }

	__super::Late_Update(fTimeDelta);
}

HRESULT CItemWood::Render()
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

HRESULT CItemWood::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Wood"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

	__super::Ready_Components();

    return S_OK;
}


CItemWood* CItemWood::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CItemWood* pInstance = new CItemWood(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CItemWood");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItemWood::Clone(void* pArg)
{
    CItemWood* pInstance = new CItemWood(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CItemWood");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItemWood::Free()
{
    __super::Free();
}
