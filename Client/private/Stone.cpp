#include "Stone.h"
#include "GameInstance.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CStone::CStone(const CStone& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CStone::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStone::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CStone::Priority_Update(_float fTimeDelta)
{
}

void CStone::Update(_float fTimeDelta)
{
}

void CStone::Late_Update(_float fTimeDelta)
{
    if (m_bRenderActive) {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }

	__super::Late_Update(fTimeDelta);
}

HRESULT CStone::Render()
{
    
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    m_pTransformCom->Bind_Resource(m_pShaderCom);
    m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);

    m_pShaderCom->Begin(0);

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    __super::Render();

    m_pShaderCom->End();
    return S_OK;
}

HRESULT CStone::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Stone"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    __super::Ready_Components();

    return S_OK;
}


CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CStone* pInstance = new CStone(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CStone");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CStone::Clone(void* pArg)
{
    CStone* pInstance = new CStone(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CStone");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CStone::Free()
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);
    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
        return;
    dynamic_cast<CTransform*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
    dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))->Set_ItemTypeAndBindTexture(ITEM_COBBLESTONE);


    __super::Free();
}
