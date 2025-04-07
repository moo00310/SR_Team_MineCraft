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

    return S_OK;
}

void CBreakableRect::Priority_Update(_float fTimeDelta)
{
    if (m_vecPositions.size() == 0) {
        Destroy();
    }

    for (auto& pair : m_Colliders)
    {
        if (!pair.second)
            continue;

        pair.second->Set_bColliderActive(false);

    }
}

void CBreakableRect::Update(_float fTimeDelta)
{
    Set_Bright();
}

void CBreakableRect::Late_Update(_float fTimeDelta)
{
    m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions, m_vecBrights);
}

HRESULT CBreakableRect::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    m_pTransformCom->Bind_Resource(m_pShaderCom);
    m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);
    
    m_pShaderCom->Begin(0);
    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;
    m_pShaderCom->End();

    for (auto& pair : m_Colliders)
    {
        if (!pair.second)
            continue;

        if (pair.second->Get_bColliderActive())
        {
            pair.second->Render_Collider(true);
        }
    }

    return S_OK;
}

void CBreakableRect::Set_BlockPositions(vector<_float3> position, ITEMNAME _name)
{
    m_Colliders.clear();
    m_itemName = _name;

    for (int i = 0; i < position.size(); ++i) {
        m_vecPositions.push_back(position[i]); //위치 넣어줌
        m_vecBrights.push_back(0.6f);

        _int3 key{};
        key.x = static_cast<_int>(position[i].x);
        key.y = static_cast<_int>(position[i].y);
        key.z = static_cast<_int>(position[i].z);



        /* For.Com_Collider */
        CComponent* pCom = nullptr;
        CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
        Desc.vRadius = { .5f, .5f, .5f };
        Desc.vOffset = { position[i].x , position[i].y, position[i].z };
        Desc.pTransformCom = m_pTransformCom;
        Desc.pOwner = this;
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
            TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&pCom), &Desc)))
        {
            MSG_BOX("Failed To Add_Collider_Cube");
            continue;
        }
        m_Colliders.emplace(key, static_cast<CCollider_Cube*>(pCom));


    }
}

HRESULT CBreakableRect::Delete_Cube(_float3 fPos)
{
    _int3 key{
    static_cast<_int>(fPos.x),
    static_cast<_int>(fPos.y),
    static_cast<_int>(fPos.z)
    };

    auto it = m_Colliders.find(key);
    if (it == m_Colliders.end())
        return E_FAIL;

    CCollider_Cube* pCollider = it->second;
    if (FAILED(Delete_Component(TEXT("Com_Collider_Cube"), pCollider)))
        return E_FAIL;

    if (FAILED(Drop_Item_OnDestroy(fPos)))
        return E_FAIL;

    if (FAILED(Play_Destroy_Effect(fPos)))
        return E_FAIL;

    for (size_t i = 0; i < m_vecPositions.size(); ++i)
    {
        if (m_vecPositions[i].x == fPos.x &&
            m_vecPositions[i].y == fPos.y &&
            m_vecPositions[i].z == fPos.z)
        {
            m_vecPositions.erase(m_vecPositions.begin() + i);
            m_vecBrights.erase(m_vecBrights.begin() + i);
            break;
        }
    }

    Safe_Release(pCollider);
    m_Colliders.erase(it);

    m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions, m_vecBrights);

    m_pGameInstance->CheckSoundStop(this, 0, 1);
    m_pGameInstance->PlaySound(TEXT("Block_BreakingFinish"), 1, fPos);

    return S_OK;
}

void CBreakableRect::Set_Bright()
{
    if (m_bChunkColliderActive)
    {
        CGameObject* pSteve{ nullptr };
        pSteve = m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve"));

        CTransform* pTransformCom{ nullptr };
        pTransformCom = static_cast<CTransform*>(pSteve->Find_Component(TEXT("Com_Transform")));
        _float3 vStevePos = { pTransformCom->Get_State(CTransform::STATE_POSITION) };


        for (int i = 0; i < m_vecPositions.size(); ++i) {
            _float3 vDiff{ vStevePos - m_vecPositions[i] };
            _float fLengthSq{ D3DXVec3LengthSq(&vDiff) };

            if (fLengthSq < 10.f) {
                m_vecBrights[i] = g_fBright + 0.2f;
            }
            else {
                m_vecBrights[i] = g_fBright;
            }
        }
    }
    else {
        for (auto& bright : m_vecBrights) {
            bright = g_fBright;
        }
    }
}

HRESULT CBreakableRect::Ready_Components()
{

    /* For.Com_VIBuffer */
    m_pVIBufferCom = CVIBuffer_Rect3DInstance::Create(m_pGraphic_Device);

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;


    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect3DInstance"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
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

    for (auto& pair : m_Colliders)
    {
        Safe_Release(pair.second);
    }

    Safe_Release(m_pShaderCom);
}
