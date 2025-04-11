#include "Grass.h"
#include "GameInstance.h"

CGrass::CGrass(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CBreakableRect(pGraphic_Device)
{
}

CGrass::CGrass(const CGrass& Prototype)
    : CBreakableRect(Prototype)
{
}

HRESULT CGrass::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGrass::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CGrass::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CGrass::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CGrass::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
        return;

    __super::Late_Update(fTimeDelta);
}

HRESULT CGrass::Render()
{
    __super::Render();
    return S_OK;
}

HRESULT CGrass::Drop_Item_OnDestroy(const _float3& fPos)
{
    wchar_t layerName[100];
    swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
        return E_FAIL;

    if (CItemRect* _copy = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
        _copy->SetPos(fPos);
        _copy->Set_Bright(fPos.y);
        _copy->Set_ItemTypeAndBindTexture(ITEMNAME_SEED);
    }

    return S_OK;
}

HRESULT CGrass::Play_Destroy_Effect(const _float3& fPos)
{
   // m_pGameInstance->Check_Sound_Stop(this, 0, SOUND_WEED);
    m_pGameInstance->Play_Sound(TEXT("Grass_dig1"), SOUND_WEED, this, 1.f, fPos);

    return S_OK;
}

HRESULT CGrass::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Grass"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;


	__super::Ready_Components();

    return S_OK;
}


CGrass* CGrass::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CGrass* pInstance = new CGrass(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CGrass");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGrass::Clone(void* pArg)
{
    CGrass* pInstance = new CGrass(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CGrass");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGrass::Free()
{
    __super::Free();
    //원형객체가 삭제될 때 Add_Gameobject해서 터지는 듯?
    //그때 GameObjectManager가 없어서 터지더라
    //클론 일때만 호출하게 하면 될 지도?

    //if (m_isCloned)
    //{
    //    int random = rand() % 100;
    //    if (random < 10) {
    //        wchar_t layerName[100];
    //        swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);
    //        if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
    //            return;
    //        dynamic_cast<CTransform*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
    //        dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))->Set_ItemTypeAndBindTexture(ITEM_SEED);
    //    }
    //}

    //이제 레퍼런스 에러 나네
    //Grass랑 튤립 자체가 레퍼런스 오류 내는가?


}
