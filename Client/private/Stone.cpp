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

    __super::Initialize(pArg);
    m_fHardness = 1.5;

    m_itemName = ITEMNAME_STONE;

    return S_OK;
}

void CStone::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CStone::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CStone::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CStone::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CStone::Drop_Item_OnDestroy(const _float3& vPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemCube* _copy = dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(vPos);
        _copy->Set_Bright(vPos.y);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_COBBLESTONE);
    }

    m_pGameInstance->Play_Sound(TEXT("Stone_dig2"), SOUND_BLOCK_DIG, this, 1.f, vPos);


    return S_OK;
}

HRESULT CStone::Play_Destroy_Effect(const _float3& fPos)
{
    CParticleEventManager::Get_Instance()->OnParticle(
        PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_DESTROY,
        fPos
    );

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



    __super::Free();
}
