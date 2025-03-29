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

HRESULT CRedTulip::Delete_Cube(_float3 fPos)
{
    for (size_t i = 0; i < m_vecPositions.size(); ++i)
    {
        if (m_vecPositions[i].x == fPos.x &&
            m_vecPositions[i].y == fPos.y &&
            m_vecPositions[i].z == fPos.z)
        {
            if (FAILED(Delete_Component(TEXT("Com_Collider_Cube"), m_Colliders[i])))
                return E_FAIL;

            wchar_t layerName[100];
            swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);
            if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
                return E_FAIL;
            dynamic_cast<CTransform*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, m_vecPositions[i]);
            dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))->Set_ItemTypeAndBindTexture(ITEM_REDTULIP);


            // 2. 벡터에서 해당 위치 제거
            m_vecPositions.erase(m_vecPositions.begin() + i);

            // 3. 콜라이더 제거
            Safe_Release(m_Colliders[i]);
            m_Colliders.erase(m_Colliders.begin() + i);

            // 4. 인스턴스 버퍼 업데이트
            m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions);

            return S_OK;
        }
    }

    return E_FAIL;
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
