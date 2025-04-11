#include "IronOre.h"
#include "GameInstance.h"

CIronOre::CIronOre(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableCube(pGraphic_Device)
{
}

CIronOre::CIronOre(const CIronOre& Prototype)
    : CBreakableCube(Prototype)
{
}

HRESULT CIronOre::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CIronOre::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_itemName = ITEMNAME_IRONORE;
    m_fHardness = 3;


    return S_OK;
}

void CIronOre::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CIronOre::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CIronOre::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CIronOre::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CIronOre::Drop_Item_OnDestroy(const _float3& vPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemRect* _copy = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(vPos);
        _copy->Set_Bright(vPos.y);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_RAWIRON);
    }

    m_pGameInstance->Play_Sound(TEXT("Stone_dig2"), SOUND_BLOCK_DIG, this, 1.f, vPos);


    return S_OK;
}

HRESULT CIronOre::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_IronOre"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;


	__super::Ready_Components();

    return S_OK;
}


CIronOre* CIronOre::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CIronOre* pInstance = new CIronOre(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CIronOre");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CIronOre::Clone(void* pArg)
{
    CIronOre* pInstance = new CIronOre(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CIronOre");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CIronOre::Free()
{
    __super::Free();
}
