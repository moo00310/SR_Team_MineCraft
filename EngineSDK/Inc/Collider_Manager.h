#pragma once

#include "Base.h"
#include "GameObject.h"
#include "LineManager.h"

BEGIN(Engine)

class CCollider_Manager final : public CBase
{
public:
	enum COLLISION_TYPE { COLLSIION_BOX, COLLSION_END };
private:
	CCollider_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCollider_Manager() = default;

public:
	HRESULT Add_CollisionGroup(_uint eCollisionGroup, class CGameObject* pGameObject);
	void Out_CollisiomGroup(_uint eCollisionGroup, class CGameObject* pGameObject);

	HRESULT Reset_ColliderGroup();

	_bool Collision_with_Group(_uint iGroupIndex, class CGameObject* pGameObject, COLLISION_TYPE eCollisionType,  _float3* pOutDistance = nullptr);
	_bool Collision_Check_Group_Multi(_uint iGroupIndex, vector<class CGameObject*>& vecDamagedObj, class CGameObject* pDamageCauser, COLLISION_TYPE eCollisionType);

	_bool Ray_Cast(
		const _float3& rayOrigin,	//���� ������
		const _float3& rayDir,		//���� ����
		_float fMaxDistanc,			//�ִ� �浹 ����
		_uint iGroupIndex,			//�浹 �� �׷�
		_Out_ _float& fDist				//�浹 �������� �Ÿ�
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