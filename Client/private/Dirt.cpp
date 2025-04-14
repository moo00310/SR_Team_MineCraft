#include "Dirt.h"
#include "GameInstance.h"

CDirt::CDirt(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CDirt::CDirt(const CDirt& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CDirt::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDirt::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_itemName = ITEMNAME_DIRT;
    m_fHardness = 0.5;


    return S_OK;
}

void CDirt::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CDirt::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CDirt::Late_Update(_float fTimeDelta)
{
    if (g_bIsScan == true)
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
            return;
    }
    else
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }

	__super::Late_Update(fTimeDelta);
}

HRESULT CDirt::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CDirt::Drop_Item_OnDestroy(const _float3& fPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemCube* _copy = dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(fPos);
        _copy->Set_Bright(fPos.y);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_DIRT);
    }

    return S_OK;
}

HRESULT CDirt::Play_Destroy_Effect(const _float3& vPos)
{
    CParticleEventManager::Get_Instance()->OnParticle(
        PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_DESTROY,
        vPos
    );

    m_pGameInstance->Play_Sound(TEXT("Gravel_dig1"), SOUND_BLOCK_DIG, this, 0.7f, vPos);

    return S_OK;
}

void CDirt::PlaySound_Hit(_float3 vPos)
{
    m_pGameInstance->Play_Sound(TEXT("Gravel_hit1"), SOUND_BLOCK_HIT, this, 0.7f, vPos);
}

void CDirt::Play_Create_Sound(_float3 vPos)
{
    m_pGameInstance->Play_Sound(TEXT("Gravel_dig1"), SOUND_BLOCK_DIG, this, 1.f, vPos);
}

HRESULT CDirt::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Dirt"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

	__super::Ready_Components();

    return S_OK;
}


CDirt* CDirt::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CDirt* pInstance = new CDirt(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CDirt");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDirt::Clone(void* pArg)
{
    CDirt* pInstance = new CDirt(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CDirt");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDirt::Free()
{

    __super::Free();
}
