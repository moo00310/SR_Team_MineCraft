#include "BreakableCube.h"
#include "MCTerrain.h"
#include "GameInstance.h"

CBreakableCube::CBreakableCube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCube(pGraphic_Device) 
{
}

CBreakableCube::CBreakableCube(const CBreakableCube& Prototype)
    : CCube(Prototype)
{
}

HRESULT CBreakableCube::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CBreakableCube::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CBreakableCube::Priority_Update(_float fTimeDelta)
{

}

void CBreakableCube::Update(_float fTimeDelta)
{

}

void CBreakableCube::Late_Update(_float fTimeDelta)
{
    if (m_pColliderCom)
        m_pColliderCom->Update_ColliderBox();
}

HRESULT CBreakableCube::Render()
{
    if (m_pColliderCom)
        m_pColliderCom->Render_ColliderBox(false);
    return S_OK;
}

HRESULT CBreakableCube::Ready_Components()
{
    /* For.Com_Collider */
    CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
    Desc.fRadiusX = .5f; Desc.fRadiusY = .5f; Desc.fRadiusZ = .5f;
    Desc.pTransformCom = m_pTransformCom;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
        return E_FAIL;

    return S_OK;
}


CBreakableCube* CBreakableCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBreakableCube* pInstance = new CBreakableCube(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBreakableCube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBreakableCube::Clone(void* pArg)
{
    CBreakableCube* pInstance = new CBreakableCube(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CBreakableCube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBreakableCube::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pColliderCom);

    dynamic_cast<CMCTerrain*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Terrain"), 0))->CheckRenderLayerObjects();
}
