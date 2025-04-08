#include "ItemRect.h"
#include "GameInstance.h"
#include "UI_Mgr.h"
#include "MCTerrain.h"
#include "Sound_Manager.h"

CItemRect::CItemRect(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCube(pGraphic_Device) 
{
}

CItemRect::CItemRect(const CItemRect& Prototype)
    : CCube(Prototype)
{
}

HRESULT CItemRect::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CItemRect::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_fUpDownSpeed = 0.005f;
    m_iUpDownFrame = 0;

    m_pTransformCom->Scaling(0.5, 0.5, 0.5);

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

void CItemRect::Priority_Update(_float fTimeDelta)
{
    m_iUpDownFrame++;
    if (m_iUpDownFrame > 20) {
        m_iUpDownFrame = 0;
        m_fUpDownSpeed *= -1;
    }
    m_pTransformCom->Turn(_float3(0, 1, 0), fTimeDelta);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(0, m_fUpDownSpeed, 0));
}

void CItemRect::Update(_float fTimeDelta)
{
    _float fDist = Compute_PlayerDistance();


    //먹어서 사라지는 거리
    if (fDist < 0.2f)
    {
        m_pGameInstance->Play_Sound(TEXT("Minecraft - Item Pop"), SOUND_ITEM, this, 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

        Destroy();
        CUI_Mgr::Get_Instance()->ItemCount_Update(m_eItemName, 1);
    }
    //플레이어한테 딸려오는 거리
    else if (fDist < 2.5f)
    {
        _float3 vStevePos = { m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{0.f, 1.5f, 0.f} };
        m_pTransformCom->Chase(vStevePos, fTimeDelta, 0.f);
    }

    if (!m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f))
        return;

    list<CCollider*> Colliders;
    Colliders = m_pTerrain->Active_Current_Chunk_Colliders(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f);

    //플레이어와 거리가 가까우면 중력적용
    m_pRigidbodyCom->Update_RayCast_InstancingObject(fTimeDelta, COLLISION_BLOCK, 0.25f);

    for (CCollider* pCollider : Colliders)
    {
        m_pGameInstance->Out_Collider_CollisiomGroup(COLLISION_BLOCK, pCollider);
    }
}

void CItemRect::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.5f))
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }

}

HRESULT CItemRect::Render()
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

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pShaderCom->End();

    return S_OK;
}

HRESULT CItemRect::Set_ItemTypeAndBindTexture(ITEMNAME _name)
{
    m_eItemName = _name;
    switch (m_eItemName)
    {
    case Client::ITEMNAME_RAWIRON:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_RawIron"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEMNAME_COAL:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Coal"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEMNAME_SAPLING:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Sapling"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEMNAME_APPLE:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Apple"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEMNAME_SEED:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Seed"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEMNAME_REDTULIP:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_RedTulip"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEMNAME_GUNPOWDER:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Gunpowder"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    case Client::ITEMNAME_ROTTENFLESH:
        /* For.Com_Texture */
        if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_rottenFlesh"),
            TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
            return E_FAIL;
        break;
    default:
        break;
    }

    return S_OK;
}

HRESULT CItemRect::Ready_Components()
{
    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_RectShader"),
        TEXT("m_pVIBufferCom_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
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

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}


_float CItemRect::Compute_PlayerDistance()
{
    _float3 vStevePos = { m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{0.f, 1.5f, 0.f} };
    _float3 vMyPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };

    _float3 vDiff{ vStevePos - vMyPos };

    _float fLength{ D3DXVec3Length(&vDiff) };

    return fLength;
}

CItemRect* CItemRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CItemRect* pInstance = new CItemRect(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CItemRect");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItemRect::Clone(void* pArg)
{
    CItemRect* pInstance = new CItemRect(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CItemRect");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItemRect::Free()
{
    __super::Free();
	Safe_Release(m_pTerrain);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRigidbodyCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pPlayerTransformCom);
}
