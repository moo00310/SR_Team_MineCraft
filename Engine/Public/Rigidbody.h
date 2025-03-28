#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRigidbody final : public CComponent
{
	//������Ʈ ��� ������ Graphic_Deviece�� ������ �޴°� �´°�??
public:
	typedef struct tagRigidbodyDesc
	{
		class CTransform*		pTransform{ nullptr };
		class CCollider*		pCollider{ nullptr };
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
	HRESULT Update_RayCast(_float fTimeDelta, _uint iCollsionGroup, _float fRayDist);
	//�ν��Ͻ� ���� ����ĳ��Ʈ ��������
	HRESULT Update_RayCast_InstancingObject(_float fTimeDelta, _uint iCollsionGroup, _float fRayDist);
public:
	_bool	Jump();
	const _float3& Get_Velocity() { return m_vReadOnly_Velocity; }
	_bool	isGround() { return m_isGround; }
private:
	void	Fall_With_Gravity(_float fTimeDelta);
	_bool	isFalling() { return m_vVelocity.y < 0.f; };
	void	Compute_Velocity(_float fTimeDelta);
private:
	class CTransform*		m_pTransform{ nullptr };
	class CCollider*		m_pCollider{ nullptr };
	_float					m_fMass{ 1.f };
	_float3					m_vVelocity{ 0.f, 0.f, 0.f };

	//���� ������ ��ġ�� ���� ��ġ�� ���ؼ� �ӵ� ��� �뵵
	_float3					m_vPrevPosition{ 0.f, 0.f, 0.f };
	_float3					m_vReadOnly_Velocity{ 0.f, 0.f, 0.f };


	//����
private:
	_bool					m_isGround{ false };
	_float					m_fJumpForce{ 8.f };

public:
	static CRigidbody* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END