#include "Furnace.h"
#include "GameInstance.h"

CFurnace::CFurnace(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CFurnace::CFurnace(const CFurnace& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CFurnace::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFurnace::Initialize(void* pArg)
{

    if (FAILED(Ready_Components()))
        return E_FAIL;

    __super::Initialize(pArg);
    m_fHardness = 1.5;

    m_itemName = ITEMNAME_FURANCE;

    return S_OK;
}

void CFurnace::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CFurnace::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CFurnace::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CFurnace::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CFurnace::Drop_Item_OnDestroy(const _float3& vPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemCube* _copy = dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(vPos);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_FURANCE);
    }

    //m_pGameInstance->Play_Sound(TEXT("Stone_dig2"), SOUND_BLOCK_DIG, this, 1.f, vPos);


    return S_OK;
}

HRESULT CFurnace::Play_Destroy_Effect(const _float3& fPos)
{
    CParticleEventManager::Get_Instance()->OnParticle(
        PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_DESTROY,
        fPos
    );

    return S_OK;
}



HRESULT CFurnace::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_FurnaceOff"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    __super::Ready_Components();

    return S_OK;
}


CFurnace* CFurnace::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CFurnace* pInstance = new CFurnace(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CFurnace");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFurnace::Clone(void* pArg)
{
    CFurnace* pInstance = new CFurnace(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CFurnace");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFurnace::Free()
{
    __super::Free();
}
