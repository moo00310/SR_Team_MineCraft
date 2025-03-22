#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRigidbody final : public CComponent
{
	//컴포넌트 상속 받으면 Graphic_Deviece를 무조건 받는게 맞는가??
public:
	typedef struct tagRigidbodyDesc
	{
		class CTransform*		pTransform{ nullptr };
		class CCollider_Cube*	pCollider_Cube{ nullptr };
		_float					fMass{ 1.f };
	}RIGIDBODY_DESC;
private:
	CRigidbody(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRigidbody(const CRigidbody& Prototype);
	virtual ~CRigidbody() = default;
public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;
	HRESULT Update(_float fTimeDelta, _uint iCollsionGroup);

public:
	_bool Jump();
private:
	void Fall_With_Gravity(_float fTimeDelta);
	_bool isFalling() { return m_vVelocity.y < 0.f; };
private:
	class CTransform*		m_pTransform{ nullptr };
	class CCollider_Cube*	m_pCollider_Cube{ nullptr };
	_float					m_fMass{ 1.f };
	_float3					m_vVelocity{ 0.f, 0.f, 0.f };

	//점프
private:
	_bool					m_isGrounded{ false };
	_float					m_fJumpForce{ 8.f };

public:
	static CRigidbody* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END