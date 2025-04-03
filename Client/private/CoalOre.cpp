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

HRESULT CCoalOre::Delete_Cube(_float3 fPos)
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

            if (CItemRect* _copy = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
                _copy->SetPos(m_vecPositions[i]);
                _copy->Set_ItemTypeAndBindTexture(ITEMNAME_COAL);

            }

            // 2. 벡터에서 해당 위치 제거
            m_vecPositions.erase(m_vecPositions.begin() + i);
            m_vecBrights.erase(m_vecBrights.begin() + i);

            // 3. 콜라이더 제거
            Safe_Release(m_Colliders[i]);
            m_Colliders.erase(m_Colliders.begin() + i);


            // 4. 인스턴스 버퍼 업데이트
            m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions, m_vecBrights);

            return S_OK;
        }
    }

    return E_FAIL;
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
