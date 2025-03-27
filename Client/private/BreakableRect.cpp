#include "BreakableRect.h"
#include "GameInstance.h"

CBreakableRect::CBreakableRect(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCube(pGraphic_Device) 
{
}

CBreakableRect::CBreakableRect(const CBreakableRect& Prototype)
    : CCube(Prototype)
{
}

HRESULT CBreakableRect::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CBreakableRect::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pColliderCom->Set_bColliderActive(true);

    return S_OK;
}

void CBreakableRect::Priority_Update(_float fTimeDelta)
{
    if (m_bChunkColliderActive) {
        m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, this);
    }
}

void CBreakableRect::Update(_float fTimeDelta)
{

}

void CBreakableRect::Late_Update(_float fTimeDelta)
{

    if (m_pColliderCom)
        m_pColliderCom->Update_Collider();

   /* if (m_pColliderCom->Get_bColliderActive())
    {
        m_pColliderCom->Render_ColliderBox(true);
    }*/

}

HRESULT CBreakableRect::Render()
{
    if (m_pColliderCom)
        m_pColliderCom->Render_Collider(false);

    return S_OK;
}

HRESULT CBreakableRect::Ready_Components()
{

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Rect3D"),
        TEXT("m_pVIBufferCom_Rect3D"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* For.Com_Collider */
    CCollider_Cube::COLLCUBE_DESC Desc{}; //ÄÝ¶óÀÌ´õ Å©±â ¼³Á¤
    Desc.fRadiusX = .5f; Desc.fRadiusY = .5f; Desc.fRadiusZ = .5f;
    _float3 pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    Desc.fOffSetX = pos.x;
    Desc.fOffSetY = pos.y;
    Desc.fOffsetZ = pos.z;
    Desc.pTransformCom = m_pTransformCom;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
        return E_FAIL;

    return S_OK;
}


CBreakableRect* CBreakableRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBreakableRect* pInstance = new CBreakableRect(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBreakableRect");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBreakableRect::Clone(void* pArg)
{
    CBreakableRect* pInstance = new CBreakableRect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CBreakableRect");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBreakableRect::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pColliderCom);
}
