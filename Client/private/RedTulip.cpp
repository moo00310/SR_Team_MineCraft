#include "RedTulip.h"
#include "GameInstance.h"

CRedTulip::CRedTulip(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableRect(pGraphic_Device)
{
}

CRedTulip::CRedTulip(const CRedTulip& Prototype)
    : CBreakableRect(Prototype)
{
}

HRESULT CRedTulip::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRedTulip::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    //m_pTransformCom->Scaling(0.5, 0.7, 0.5);

    return S_OK;
}

void CRedTulip::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CRedTulip::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CRedTulip::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;

    __super::Late_Update(fTimeDelta);
}

HRESULT CRedTulip::Render()
{
    __super::Render();
    
    return S_OK;
}

HRESULT CRedTulip::Drop_Item_OnDestroy(const _float3& fPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemRect* _copy = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(fPos);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_REDTULIP);
    }

    return S_OK;
}

HRESULT CRedTulip::Play_Destroy_Effect(const _float3& fPos)
{
    m_pGameInstance->CheckSoundStop(this, 0, 1);
    m_pGameInstance->PlaySound(TEXT("Grass_dig1"), 1, fPos);

    return S_OK;
}


HRESULT CRedTulip::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_RedTulip"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;


	__super::Ready_Components();

    return S_OK;
}


CRedTulip* CRedTulip::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CRedTulip* pInstance = new CRedTulip(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CRedTulip");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CRedTulip::Clone(void* pArg)
{
    CRedTulip* pInstance = new CRedTulip(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CRedTulip");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRedTulip::Free()
{
    __super::Free();
    //원형객체가 삭제될 때 Add_Gameobject해서 터지는 듯?
    //그때 GameObjectManager가 없어서 터지더라
    ////클론 일때만 호출하게 하면 될 지도?
    //if (m_isCloned)
    //{
    //    wchar_t layerName[100];
    //    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);
    //    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
    //        return;
    //    dynamic_cast<CTransform*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
    //    dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))->Set_ItemTypeAndBindTexture(ITEM_REDTULIP);

    //}


}
