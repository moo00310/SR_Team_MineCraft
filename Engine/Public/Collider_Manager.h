#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CCollider_Manager final : public CBase
{
public:
	enum COLLISION_GROUP { COLLISION_PLAYER, COLLISION_MONSTER, COLLISION_OBJECT, COLLISION_BLOCK, COLLISION_PUSH, COLLISION_GROUPEND };
	enum COLLISION_TYPE {COLLISION_RECT, COLLSIION_BOX, COLLSION_END };
private:
	CCollider_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CCollider_Manager() = default;

public:
	HRESULT Add_CollisionGroup(COLLISION_GROUP eCollisionGroup, class CGameObject* pGameObject);
	void Out_CollisiomGroup(COLLISION_GROUP eCollisionGroup, class CGameObject* pGameObject);


	HRESULT Reset_ColliderGroup();

	_bool Collision_with_Group(COLLISION_GROUP eGroup, class CGameObject* pGameObject, COLLISION_TYPE eCollisionType,  _float3* pOutDistance = nullptr);
	_bool Collision_Check_Group_Multi(COLLISION_GROUP eGroup, vector<class CGameObject*>& vecDamagedObj, class CGameObject* pDamageCauser, COLLISION_TYPE eCollisionType);
	_bool Ray_Cast(const _float4x4* matWorld, _float3 vOrigin, _float3 vDir, _float fLength, CCollider_Manager::COLLISION_GROUP eGroup);
private:
	list<class CGameObject*> m_GameObjects[COLLISION_GROUPEND];
	typedef list<class CGameObject*> GAMEOBJECTS;
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr }; //라인 디버깅 하려고 가져왔음

public:
	static CCollider_Manager* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};
END