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

    m_fUpDownSpeed = 0.005f;
    m_iUpDownFrame = 0;

    m_pTransformCom->Scaling(0.3f, 0.3f, 0.3f);

    return S_OK;
}

void CItemCube::Priority_Update(_float fTimeDelta)
{
    m_iUpDownFrame++;
    if (m_iUpDownFrame > 20) {
        m_iUpDownFrame = 0;
        m_fUpDownSpeed *= -1;
    }
    m_pTransformCom->Turn(_float3(0, 1, 0), fTimeDelta);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(0, m_fUpDownSpeed, 0));
}

void CItemCube::Update(_float fTimeDelta)
{
    //플레이어와 거리 계산해서 멀면 비활성화
    CGameObject* pSteve{ nullptr };
    pSteve = m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve"));

    CTransform* pTransformCom{ nullptr };
    pTransformCom = static_cast<CTransform*>(pSteve->Find_Component(TEXT("Com_Transform")));

    _float3 vStevePos = { pTransformCom->Get_State(CTransform::STATE_POSITION) };
    _float3 vMyPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };

    _float3 vDiff{ vStevePos - vMyPos };

    _float fLengthSq{ D3DXVec3LengthSq(&vDiff) };

    if (fLengthSq < 20.f)
    {
        //플레이어와 거리가 가까우면 중력적용
        m_pRigidbodyCom->Update_RayCast_InstancingObject(fTimeDelta, COLLISION_BLOCK, 0.5f);
    }

}

void CItemCube::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.5f))
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }

}

HRESULT CItemCube::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemCube::Set_ItemTypeAndBindTexture(ITEMTYPE _type)
{
    m_eItemType = _type;
    switch (m_eItemType)
    {
    case Client::ITEM_DIRT:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Dirt"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEM_COBBLESTONE:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_CobbleStone"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEM_RAWIRON:
        break;
    case Client::ITEM_COAL:
        break;
    case Client::ITEM_WOOD:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Wood"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEM_SAPLING:
        break;
    case Client::ITEM_APPLE:
        break;
    case Client::ITEM_SEED:
        break;
    case Client::ITEM_REDTULIP:
        break;
    case Client::ITEM_END:
        break;
    default:
        break;
    }

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

    ///* For.Com_Collider */
    //CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
    //Desc.fRadiusX = .5f; Desc.fRadiusY = .5f; Desc.fRadiusZ = .5f;
    //Desc.pTransformCom = m_pTransformCom;
    //if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
    //    TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
    //    return E_FAIL;

	/* For.Com_Rigidbody */
	CRigidbody::RIGIDBODY_DESC RigidbodyDesc{};
	RigidbodyDesc.pTransform = m_pTransformCom;
	//RigidbodyDesc.pCollider_Cube = m_pColliderCom;
	RigidbodyDesc.fMass = 1.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Rigidbody"),
		TEXT("Com_Rigidbody"), reinterpret_cast<CComponent**>(&m_pRigidbodyCom), &RigidbodyDesc)))
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
    Safe_Release(m_pRigidbodyCom);
    //Safe_Release(m_pColliderCom);
}
