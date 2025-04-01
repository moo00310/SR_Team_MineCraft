#include "ItemRect.h"
#include "GameInstance.h"

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
    //�÷��̾�� �Ÿ� ����ؼ� �ָ� ��Ȱ��ȭ
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
        //�÷��̾�� �Ÿ��� ������ �߷�����
        m_pRigidbodyCom->Update_RayCast_InstancingObject(fTimeDelta, COLLISION_BLOCK, 0.5f);
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
    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    /* ������ �׸���. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

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
    default:
        break;
    }

    return S_OK;
}

HRESULT CItemRect::Ready_Components()
{
    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
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

    return S_OK;
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
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRigidbodyCom);
}
