#include "CobbleStone.h"
#include "GameInstance.h"

CCobbleStone::CCobbleStone(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CCobbleStone::CCobbleStone(const CCobbleStone& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CCobbleStone::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCobbleStone::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_itemName = ITEMNAME_DIRT;
    m_fHardness = 0.5;


    return S_OK;
}

void CCobbleStone::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CCobbleStone::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CCobbleStone::Late_Update(_float fTimeDelta)
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

HRESULT CCobbleStone::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CCobbleStone::Drop_Item_OnDestroy(const _float3& fPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemCube* _copy = dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(fPos);
        _copy->Set_Bright(fPos.y);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_COBBLESTONE);
    }

    return S_OK;
}

// 매개변수 만들어서 던지면 되긴하거든? 그럼 노가다긴함
// 겟셋 만들어서 맴버변수로 분기 나눠도 되고 
// 매개변수로 할까 맴버변수 파서 크리퍼에서 바꿀까 
HRESULT CCobbleStone::Play_Destroy_Effect(const _float3& vPos)
{
    CParticleEventManager::Get_Instance()->OnParticle(
        PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING,
        vPos
    );

    if (!isCreeperBoom)   
        m_pGameInstance->Play_Sound(TEXT("Stone_dig2"), SOUND_BLOCK_DIG, this, 0.5f, vPos);

    return S_OK;
}

void CCobbleStone::PlaySound_Hit(_float3 vPos)
{
    m_pGameInstance->Play_Sound(TEXT("Stone_hit1"), SOUND_BLOCK_HIT, this, 0.5f, vPos);
}

void CCobbleStone::Play_Create_Sound(_float3 vPos)
{
    m_pGameInstance->Play_Sound(TEXT("Stone_dig2"), SOUND_BLOCK_DIG, this, 1.f, vPos);
}

HRESULT CCobbleStone::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_CobbleStone"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

	__super::Ready_Components();

    return S_OK;
}


CCobbleStone* CCobbleStone::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCobbleStone* pInstance = new CCobbleStone(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCobbleStone");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCobbleStone::Clone(void* pArg)
{
    CCobbleStone* pInstance = new CCobbleStone(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CCobbleStone");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCobbleStone::Free()
{

    __super::Free();
}
