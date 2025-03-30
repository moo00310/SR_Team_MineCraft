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
	//게임 오브젝트가 가지고 있는 모든 콜라이더를 등록 하는 함수
	HRESULT Add_CollisionGroup(_uint iGroupIndex, class CGameObject* pGameObject);
	//콜라이더 하나를 등록 하는 함수
	HRESULT Add_Collider_CollisionGroup(_uint iGroupIndex, class CCollider* pCollider);


	void Out_CollisiomGroup(_uint iGroupIndex, class CGameObject* pGameObject);

	HRESULT Reset_ColliderGroup();

	class CGameObject* Collision_Check_with_Group(_uint iGroupIndex, CComponent* pCollider, COLLISION_TYPE eCollisionType, _Out_ _float3* pOutDepth = nullptr, _Out_ CCollider::COLLISION_DIR* pOutDir = nullptr);
	
	_bool Collision_Check_Group_Multi(
		_uint iGroupIndex,
		list<CCollider_Cube::COLLISION_INFO>& CollisionObjects,
		CComponent* pCollider,
		COLLISION_TYPE eCollisionType
	);

	//일반 버전
	CGameObject* Ray_Cast(
		const _float3& rayOrigin,					//레이 시작점
		const _float3& rayDir,						//레이 방향
		_float fMaxDistanc,							//최대 충돌 범위
		_uint iGroupIndex,							//충돌 할 그룹
		_Out_ _float* pDist							//충돌 지점과의 거리
	);

	//인스턴싱 오브젝트 버전
	CGameObject* Ray_Cast_InstancedObjects(
		const _float3& rayOrigin,					//레이 시작점
		const _float3& rayDir,						//레이 방향
		_float fMaxDistanc,							//최대 충돌 범위
		_uint iGroupIndex,							//충돌 할 그룹
		_Out_ _float* fDist = nullptr,				//충돌 지점과의 거리
		_Out_ _float3* pOutCollision_Dir = nullptr, //충돌 방향
		_Out_ CComponent** ppOutCollider = nullptr	//충돌한 콜라이더
	);

	//인스턴싱 오브젝트 버전
	CGameObject* Ray_Cast_MultiGroup_InstancedObjects(
		const _float3& rayOrigin,					//레이 시작점
		const _float3& rayDir,						//레이 방향
		_float fMaxDistanc,							//최대 충돌 범위
		const vector<_uint>& vGroupIndices,			//충돌 할 그룹 벡터
		_Out_ _float* pDist = nullptr,				//충돌 지점과의 거리
		_Out_ _float3* pOutCollision_Dir = nullptr,	//충돌 방향
		_Out_ CComponent** ppOutCollider = nullptr	//충돌한 콜라이더
	);

	void  Render();
private:
	CLineManager*	m_pLineManager{ nullptr };

private:
	_uint						m_iNumGroups = {};
	list<class CCollider*>*		m_pColliders{ nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr }; //라인 디버깅 하려고 가져왔음

public:
	HRESULT						Initialize(_uint iNumGroups);
	static CCollider_Manager*	Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint CollisionGroups);
	virtual void				Free() override;
};
END