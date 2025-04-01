#pragma once

#include "Collider.h"
#include "Transform.h"
BEGIN(Engine)

class ENGINE_DLL CCollider_Capsule final : public CCollider
{

public:
    typedef struct tagCollisionInfo
    {
        class CGameObject* pGameObject{ nullptr };   // 충돌한 오브젝트
        COLLISION_DIR eCollisionDir{ COLLISION_DIR::NONE };  // 충돌 방향
        _float3 vDepth{ 0.f, 0.f, 0.f };  // 충돌 깊이
    } COLLISION_INFO;

public:
    typedef struct tagCollisionCapsuleDesc : public COLLDESC
    {
        _float fRadius{ 0.5f };
        _float fHeight{ 1.0f };  // 캡슐의 높이
        _float3 vfOffset{0.f, 0.f, 0.f};
        //class CTransform* pTransformCom = { nullptr };
    } COLLCAPSULE_DESC;

private:
    CCollider_Capsule(LPDIRECT3DDEVICE9 pGraphic_Device);
    CCollider_Capsule(const CCollider_Capsule& Prototype);
    virtual ~CCollider_Capsule() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    virtual HRESULT Update_Collider()override;
    virtual HRESULT Render_Collider(_bool isHit)override;

    _bool Collision_Check(CCollider_Cube* pTarget, _Out_ _float3* pOutDistance = nullptr, _Out_ CCollider::COLLISION_DIR* pOutDir = nullptr, _Out_ _float3* pOutNormal = nullptr) override;
public:
    bool Get_bColliderActive() { return m_bColliderActive; }
    void Set_bColliderActive(bool _b) { m_bColliderActive = _b; }

private:
    _float3 m_vPoint[18]; // 캡슐은 원 2개와 2개의 점을 갖는 16개의 점으로 정의됨

private:
    class CTransform* m_pTransformCom = { nullptr };

    _float  m_fRadius{ 0.5f };
    _float  m_fHeight{ 1.0f }; // 캡슐의 높이
    _float3 m_vOffset{};

public:
    static CCollider_Capsule* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CComponent* Clone(void* pArg = nullptr) override;
    virtual void Free() override;
};
END