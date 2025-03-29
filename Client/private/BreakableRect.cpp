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

    // 플레이어 밑에 있는 청크면 충돌 매니저에 올림
    if (m_bChunkColliderActive)
    {
        CGameObject* pSteve{ nullptr };
        pSteve = m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve"));

        CTransform* pTransformCom{ nullptr };
        pTransformCom = static_cast<CTransform*>(pSteve->Find_Component(TEXT("Com_Transform")));
        _float3 vStevePos = { pTransformCom->Get_State(CTransform::STATE_POSITION) };

        //플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
        for (CCollider_Cube* pCollider : m_Colliders)
        {
            _float3 vColliderPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset() };

            _float3 vDiff{ vStevePos - vColliderPos };

            _float fLengthSq{ D3DXVec3LengthSq(&vDiff) };

            if (fLengthSq < 30.f)
            {
                //플레이어와 거리가 가까우면
                m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, pCollider);

                pCollider->Set_bColliderActive(true);
            }
            else
            {
                pCollider->Set_bColliderActive(false);
            }
        }
    }
    else
    {
        for (CCollider_Cube* pCollider : m_Colliders)
        {
            pCollider->Set_bColliderActive(false);
        }
    }
}

void CBreakableRect::Update(_float fTimeDelta)
{

}

void CBreakableRect::Late_Update(_float fTimeDelta)
{


}

HRESULT CBreakableRect::Render()
{
    for (int i = 0; i < m_Colliders.size(); ++i)
    {
        if (m_Colliders[i]->Get_bColliderActive())
        {
            m_Colliders[i]->Render_Collider(true);

        }
    }
    return S_OK;
}

void CBreakableRect::Set_BlockPositions(vector<_float3> position)
{
    m_Colliders.clear();
    m_Colliders.resize(position.size());

    for (int i = 0; i < position.size(); ++i) {
        m_vecPositions.push_back(position[i]); //위치 넣어줌

        /* For.Com_Collider */
        CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
        Desc.vRadius = { .5f, .5f, .5f };
        Desc.vOffset = { position[i].x , position[i].y, position[i].z };
        Desc.pTransformCom = m_pTransformCom;
        Desc.pOwner = this;
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
            TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_Colliders[i]), &Desc)))
        {

        }

    }
}

HRESULT CBreakableRect::Delete_Cube(_float3 fPos)
{
    return S_OK;
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

    for (CCollider_Cube* m_Colliders : m_Colliders)
        Safe_Release(m_Colliders);

    Safe_Release(m_pShaderCom);
}
