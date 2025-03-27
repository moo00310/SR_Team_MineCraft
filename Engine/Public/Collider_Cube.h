#pragma once

#include "Collider.h"
#include "Transform.h"
BEGIN(Engine)


class ENGINE_DLL CCollider_Cube final : public CCollider
{
public:
	typedef struct tagCollisionInfo
	{
		class CGameObject* pGameObject{ nullptr };   // 충돌한 오브젝트
		COLLISION_DIR eCollisionDir{ COLLISION_DIR::NONE };  // 충돌 방향
		_float3 vDepth{ 0.f, 0.f, 0.f };  // 충돌 깊이
	}COLLISION_INFO;

public:
	typedef struct tagCollisionCubeDesc
	{
		_float	fRadiusX = 0.5f, fRadiusY = 0.5f,  fRadiusZ = 0.5f;
		_float	fOffSetX = 0.f, fOffSetY = 0.f,  fOffsetZ = 0.f;
		class CTransform* pTransformCom = { nullptr };
	}COLLCUBE_DESC;

private:
	CCollider_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCollider_Cube(const CCollider_Cube& Prototype);
	virtual ~CCollider_Cube() = default;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;
	virtual HRESULT Update_Collider()override;
	virtual HRESULT Render_Collider(_bool isHit)override;
	_bool Collision_Check(CCollider_Cube* pTarget, _Out_ _float3* pOutDistance, _Out_ CCollider::COLLISION_DIR* pOutDir) override;

public:
	COLLCUBE_DESC&	Get_Desc() { return m_StateDesc; }
	void			Set_Desc(COLLCUBE_DESC& Desc) { m_StateDesc = Desc; }

	_float3 GetMin() const;
	_float3 GetMax() const;

	class CTransform* Get_Transform() { return m_pTransformCom; }

	bool Get_bColliderActive() { return m_bColliderActive; }
	void Set_bColliderActive(bool _b) { m_bColliderActive = _b; }

private:
	_float3						m_vPoint[8];

protected:
	COLLCUBE_DESC				m_StateDesc;





public:
	static CCollider_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END