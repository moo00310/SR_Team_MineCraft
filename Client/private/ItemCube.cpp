#include "ItemCube.h"
#include "GameInstance.h"

CItemCube::CItemCube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCube(pGraphic_Device) 
{
}

CItemCube::CItemCube(const CItemCube& Prototype)
    : CCube(Prototype)
{
}

HRESULT CItemCube::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CItemCube::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CItemCube::Priority_Update(_float fTimeDelta)
{
}

void CItemCube::Update(_float fTimeDelta)
{
}

void CItemCube::Late_Update(_float fTimeDelta)
{
    if (m_pColliderCom)
        m_pColliderCom->Update_ColliderBox();
}

HRESULT CItemCube::Render()
{
    if (m_pColliderCom)
        m_pColliderCom->Render_ColliderBox(false);
    return S_OK;
}

HRESULT CItemCube::Ready_Components()
{
    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
        TEXT("m_pVIBufferCom_Cube"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* For.Com_Collider */
    CCollider_Cube::COLLCUBE_DESC Desc{}; //�ݶ��̴� ũ�� ����
    Desc.fRadiusX = .5f; Desc.fRadiusY = .5f; Desc.fRadiusZ = .5f;
    Desc.pTransformCom = m_pTransformCom;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
        return E_FAIL;

    return S_OK;
}


CItemCube* CItemCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CItemCube* pInstance = new CItemCube(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CItemCube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItemCube::Clone(void* pArg)
{
    CItemCube* pInstance = new CItemCube(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CItemCube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItemCube::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pColliderCom);
}
