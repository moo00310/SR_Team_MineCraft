#include "ItemCube.h"
#include "GameInstance.h"
#include "UI_Mgr.h"
#include "MCTerrain.h"

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

	m_pTerrain = static_cast<CMCTerrain*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Terrain")));
	Safe_AddRef(m_pTerrain);

    CGameObject* pSteve{ nullptr };
    pSteve = m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve"));
    CTransform* pTransformCom{ nullptr };
    m_pPlayerTransformCom = static_cast<CTransform*>(pSteve->Find_Component(TEXT("Com_Transform")));
    Safe_AddRef(m_pPlayerTransformCom);


    const _float fPower{ 0.5f };
    const _float fHorizonPower{ (rand() % 11 - 5) * fPower };
    const _float fVerticalPower{ (rand() % 11 + 3) * fPower };
    m_pRigidbodyCom->Knock_back(_float3{ fHorizonPower,fVerticalPower , fHorizonPower });


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
    _float fDist = Compute_PlayerDistance();


    if (fDist < 0.1f)
    {
        CUIDropItem* dropItem = (CUIDropItem*)m_pGameInstance->PushPool(LEVEL_YU,		// ���� ��.
            PROTOTYPE_GAMEOBJECT_UI_DROP_ITEM,	// ������ ������Ÿ��.
            LEVEL_YU,		// ������ ��.
            LAYER_UI_POOL	// �ֵ������Ʈ�� �߰��� ���̾�.
            );

        dropItem->SetTransform(m_pTransformCom->WorldToScreen());
        dropItem->SetDirection({0.f, 1.f});

        Destroy();
        CUI_Mgr::Get_Instance()->ItemCount_Update(m_eItemName, 1);
    }
    else if (fDist < 1.5f)
    {
        _float3 vStevePos = { m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{0.f, 1.5f, 0.f} };
        m_pTransformCom->Chase(vStevePos, fTimeDelta, 0.f);
    }
    else if (fDist < 5.f)
    {
        list<CCollider*> Colliders;
        Colliders = m_pTerrain->Active_Near_Chunk_Colliders(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f);

        //�÷��̾�� �Ÿ��� ������ �߷�����
        m_pRigidbodyCom->Update_RayCast_InstancingObject(fTimeDelta, COLLISION_BLOCK, 0.25f);

        for (CCollider* pCollider : Colliders)
        {
			m_pGameInstance->Out_Collider_CollisiomGroup(COLLISION_BLOCK, pCollider);
        }
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

    m_pTransformCom->Bind_Resource(m_pShaderCom);
    m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);
    m_pShaderCom->SetFloat("g_Bright", g_fBright + 0.2f);
    m_pShaderCom->Begin(1);

    /* ������ �׸���. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pShaderCom->End();

    return S_OK;
}

HRESULT CItemCube::Set_ItemTypeAndBindTexture(ITEMNAME _name)
{
    m_eItemName = _name;
    switch (m_eItemName)
    {
    case Client::ITEMNAME_DIRT:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Dirt"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEMNAME_COBBLESTONE:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_CobbleStone"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEMNAME_WOOD:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Wood"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
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

	/* For.Com_Rigidbody */
	CRigidbody::RIGIDBODY_DESC RigidbodyDesc{};
	RigidbodyDesc.pTransform = m_pTransformCom;
	//RigidbodyDesc.pCollider_Cube = m_pColliderCom;
	RigidbodyDesc.fMass = 1.f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Rigidbody"),
		TEXT("Com_Rigidbody"), reinterpret_cast<CComponent**>(&m_pRigidbodyCom), &RigidbodyDesc)))
		return E_FAIL;

    // ���̴� ������Ʈ
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Cube"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;


    return S_OK;
}


_float CItemCube::Compute_PlayerDistance()
{
    _float3 vStevePos = { m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{0.f, 1.5f, 0.f} };
    _float3 vMyPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };

    _float3 vDiff{ vStevePos - vMyPos };

    _float fLength{ D3DXVec3Length(&vDiff) };

    return fLength;
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
	Safe_Release(m_pTerrain);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRigidbodyCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pPlayerTransformCom);
}
