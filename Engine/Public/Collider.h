#pragma once
#include "Engine_Defines.h"
#include "Component.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
public:
    enum class COLLISION_DIR { NONE, LEFT, RIGHT, UP, DOWN, BACK, FRONT };

protected:
	typedef struct tagColliderDesc
	{
		class CTransform*	pTransformCom;
		class CGameObject*	pOwner;
	}COLLDESC;

protected:
	CCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	virtual HRESULT Update_Collider();
	virtual HRESULT Render_Collider(_bool isHit);

	virtual _bool Collision_Check(class CCollider_Cube* pTarget, _Out_ _float3* pOutDistance = nullptr, _Out_ CCollider::COLLISION_DIR* pOutDir = nullptr, _Out_ _float3* pOutNormal = nullptr) = 0;
	class CGameObject* Get_Owner() { return m_pOwner; }
	class CTransform* Get_Transform() { return m_pTransformCom; }

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

	_bool m_bColliderActive{ false };

protected:

	class CTransform* m_pTransformCom = { nullptr };
	class CGameObject* m_pOwner{ nullptr };

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END

