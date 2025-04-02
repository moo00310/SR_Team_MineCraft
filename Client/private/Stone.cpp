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

HRESULT CStone::Delete_Cube(_float3 fPos)
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
            if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
                return E_FAIL;

            if (CItemCube* _copy = dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
                _copy->SetPos(m_vecPositions[i]);
                _copy->Set_ItemTypeAndBindTexture(ITEMNAME_COBBLESTONE);
                _copy->Set_Bright(m_vecBrights[i]);

            }

            // 2. 벡터에서 해당 위치 제거
            m_vecPositions.erase(m_vecPositions.begin() + i);
            m_vecBrights.erase(m_vecBrights.begin() + i);

            // 3. 콜라이더 제거
            Safe_Release(m_Colliders[i]);
            m_Colliders.erase(m_Colliders.begin() + i);

            if (m_vecPositions.size() == 0) {
            }

            // 4. 인스턴스 버퍼 업데이트
            m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions, m_vecBrights);

            return S_OK;
        }
    }

    return E_FAIL;
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
