#include "OakWood.h"
#include "GameInstance.h"

COakWood::COakWood(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

COakWood::COakWood(const COakWood& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT COakWood::Initialize_Prototype()
{
    return S_OK;
}

HRESULT COakWood::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_itemName = ITEMNAME_OAKPLANKS;
    m_fHardness = 1.8;


    return S_OK;
}

void COakWood::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void COakWood::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void COakWood::Late_Update(_float fTimeDelta)
{
	
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;
    __super::Late_Update(fTimeDelta);
}

HRESULT COakWood::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT COakWood::Drop_Item_OnDestroy(const _float3& fPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemCube* _copy = dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(fPos);
        _copy->Set_Bright(fPos.y);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_OAKPLANKS);
    }

    return S_OK;
}

HRESULT COakWood::Play_Destroy_Effect(const _float3& vPos)
{
    CParticleEventManager::Get_Instance()->OnParticle(
        PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_DESTROY,
        vPos
    );

    m_pGameInstance->Play_Sound(TEXT("Gravel_dig1"), SOUND_BLOCK_DIG, this, 1.f, vPos);

    return S_OK;
}

void COakWood::PlaySound_Hit(_float3 vPos)
{
    m_pGameInstance->Play_Sound(TEXT("Gravel_hit1"), SOUND_BLOCK_HIT, this, 1.f, vPos);

}

void COakWood::Play_Create_Sound(_float3 vPos)
{
    m_pGameInstance->Play_Sound(TEXT("Gravel_dig1"), SOUND_BLOCK_DIG, this, 1.f, vPos);
}

HRESULT COakWood::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_OakWood"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    __super::Ready_Components();

    return S_OK;
}


COakWood* COakWood::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    COakWood* pInstance = new COakWood(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : COakWood");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* COakWood::Clone(void* pArg)
{
    COakWood* pInstance = new COakWood(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : COakWood");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void COakWood::Free()
{
    __super::Free();



}
