#include "GrassDirt.h"
#include "GameInstance.h"

CGrassDirt::CGrassDirt(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CGrassDirt::CGrassDirt(const CGrassDirt& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CGrassDirt::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGrassDirt::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_itemName = ITEMNAME_GRASSDIRT;
    m_fHardness = 0.6f;


    return S_OK;
}

void CGrassDirt::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CGrassDirt::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CGrassDirt::Late_Update(_float fTimeDelta)
{
	
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;
    __super::Late_Update(fTimeDelta);
}

HRESULT CGrassDirt::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT CGrassDirt::Drop_Item_OnDestroy(const _float3& fPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemCube* _copy = dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(fPos);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_DIRT);
    }

    return S_OK;
}

HRESULT CGrassDirt::Play_Destroy_Effect(const _float3& fPos)
{
    CParticleEventManager::Get_Instance()->OnParticle(
        PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_DESTROY,
        fPos
    );
    return S_OK;
}

HRESULT CGrassDirt::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_GrassDirt"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    __super::Ready_Components();

    return S_OK;
}


CGrassDirt* CGrassDirt::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CGrassDirt* pInstance = new CGrassDirt(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CGrassDirt");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGrassDirt::Clone(void* pArg)
{
    CGrassDirt* pInstance = new CGrassDirt(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CGrassDirt");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGrassDirt::Free()
{
    __super::Free();



}
