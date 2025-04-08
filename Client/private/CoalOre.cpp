#include "CoalOre.h"
#include "GameInstance.h"

CCoalOre::CCoalOre(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CCoalOre::CCoalOre(const CCoalOre& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CCoalOre::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCoalOre::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_itemName = ITEMNAME_COALORE;
    m_fHardness = 3;


    return S_OK;
}

void CCoalOre::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CCoalOre::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CCoalOre::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CCoalOre::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CCoalOre::Drop_Item_OnDestroy(const _float3& fPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemRect* _copy = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(fPos);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_COAL);
    }

    return S_OK;
}

HRESULT CCoalOre::Play_Destroy_Effect(const _float3& vPos)
{
    // 필요 시 파티클 추가
    // CParticleEventManager::Get_Instance()->OnParticle(...);

    m_pGameInstance->Play_Sound(TEXT("Stone_dig2"), SOUND_BLOCK_DIG, this, 1.f, vPos);


    return S_OK;
}


HRESULT CCoalOre::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_CoalOre"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    __super::Ready_Components();

    return S_OK;
}


CCoalOre* CCoalOre::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCoalOre* pInstance = new CCoalOre(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCoalOre");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCoalOre::Clone(void* pArg)
{
    CCoalOre* pInstance = new CCoalOre(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CCoalOre");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCoalOre::Free()
{


    __super::Free();
}
