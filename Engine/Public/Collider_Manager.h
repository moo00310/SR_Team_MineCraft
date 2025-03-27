#pragma once

#include "Base.h"
#include "GameObject.h"
#include "LineManager.h"
#include "Collider.h"
#include "Collider_Cube.h"
#include "Collider_Capsule.h"

BEGIN(Engine)

class CCollider_Manager final : public CBase
{
public:
	enum COLLISION_TYPE { COLLSIION_CUBE, COLLSIION_CAPSULE, COLLSION_END };
private:
	CCollider_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCollider_Manager() = default;

public:
	HRESULT Add_CollisionGroup(_uint eCollisionGroup, class CGameObject* pGameObject);
	void Out_CollisiomGroup(_uint eCollisionGroup, class CGameObject* pGameObject);

	HRESULT Reset_ColliderGroup();

	class CGameObject* Collision_Check_with_Group(_uint iGroupIndex, CComponent* pCollider, COLLISION_TYPE eCollisionType, _Out_ _float3* pOutDepth = nullptr, _Out_ CCollider::COLLISION_DIR* pOutDir = nullptr);
	
	_bool Collision_Check_Group_Multi(
		_uint iGroupIndex,
		list<CCollider_Cube::COLLISION_INFO>& CollisionObjects,
		CComponent* pCollider,
		COLLISION_TYPE eCollisionType
	);

	CGameObject* Ray_Cast(
		const _float3& rayOrigin,					//���� ������
		const _float3& rayDir,						//���� ����
		_float fMaxDistanc,							//�ִ� �浹 ����
		_uint iGroupIndex,							//�浹 �� �׷�
		_Out_ _float& fDist						//�浹 �������� �Ÿ�
	);

	//�ν��Ͻ̵� ������ Ʈ�������� �̿��ؼ� �浹 ó���Ұ�
	CGameObject* Ray_Cast_InstancingObject(
		const _float3& rayOrigin,					//���� ������
		const _float3& rayDir,						//���� ����
		_float fMaxDistanc,							//�ִ� �浹 ����
		_uint iGroupIndex,							//�浹 �� �׷�
		_Out_ _float& fDist,					//�浹 �������� �Ÿ�
		_Out_ CComponent** ppOutCollider
	);

	void  Render();
private:
	CLineManager*	m_pLineManager{ nullptr };

private:
	_uint						m_iNumGroups = {};
	list<class CGameObject*>*	m_pGameObjects{ nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr }; //���� ����� �Ϸ��� ��������

public:
	HRESULT						Initialize(_uint iNumGroups);
	static CCollider_Manager*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint CollisionGroups);
	virtual void				Free() override;
};
END