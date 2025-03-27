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
    typedef struct tagCollisionCapsuleDesc
    {
        _float fRadius = 0.5f;
        _float fHeight = 1.0f;  // 캡슐의 높이
        _float fOffsetX = 0.f, fOffsetY = 0.f, fOffsetZ = 0.f;
        class CTransform* pTransformCom = { nullptr };
    } COLLCAPSULE_DESC;

private:
    CCollider_Capsule(LPDIRECT3DDEVICE9 pGraphic_Device);
    CCollider_Capsule(const CCollider_Capsule& Prototype);
    virtual ~CCollider_Capsule() = default;

public:
    HRESULT Initialize_Prototype();
    HRESULT Initialize(void* pArg) override;
    HRESULT Update_ColliderCapsule();
    HRESULT Render_ColliderCapsule(_bool isHit);

    _bool Collision_Check(CCollider_Capsule* pTarget, _Out_ _float3* pOutDistance = nullptr, _Out_ COLLISION_DIR* pOutDir = nullptr);
    _bool Collision_Check_Cube(class CCollider_Cube* pTarget, _float3* pOutDistance, COLLISION_DIR* pOutDir);


public:
    COLLCAPSULE_DESC& Get_Desc() { return m_StateDesc; }
    void Set_Desc(COLLCAPSULE_DESC& Desc) { m_StateDesc = Desc; }

    bool Get_bColliderActive() { return m_bColliderActive; }
    void Set_bColliderActive(bool _b) { m_bColliderActive = _b; }

private:
    _float3 m_vPoint[16]; // 캡슐은 원 2개와 2개의 점을 갖는 16개의 점으로 정의됨

protected:
    COLLCAPSULE_DESC m_StateDesc;

private:
    class CTransform* m_pTransformCom = { nullptr };

protected:
    LPDIRECT3DVERTEXBUFFER9 m_pVB = { nullptr };
    _uint m_iNumVertices = {};
    _uint m_iStride = {}; /* 정점의 크기(byte) */
    _ulong m_dwFVF = {};
    D3DPRIMITIVETYPE m_ePrimitiveType = {};
    _uint m_iNumPrimitive = {};

protected:
    LPDIRECT3DINDEXBUFFER9 m_pIB = { nullptr };
    _uint m_iIndicesByte = {};
    D3DFORMAT m_eIndexFormat = {};

    bool m_bColliderActive;

public:
    static CCollider_Capsule* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CComponent* Clone(void* pArg = nullptr) override;
    virtual void Free() override;
};
END