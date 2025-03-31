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

	//콜라이더를 그룹에서 제거하는 함수
	void Out_CollisiomGroup(_uint iGroupIndex, class CGameObject* pGameObject);

	//콜라이더 그룹을 초기화 하는 함수
	HRESULT Reset_ColliderGroup();

	// 특정 그룹 내에서 충돌을 검사하고, 첫 번째로 감지된 충돌 객체를 반환합니다.
	class CGameObject* Collision_Check_with_Group(
		_uint iGroupIndex,									// 검사할 그룹의 인덱스
		CComponent* pCollider,								// 충돌을 검사할 대상 Collider
		COLLISION_TYPE eCollisionType,						// 충돌할 콜라이더 종류
		_Out_ _float3* pOutDepth = nullptr,					// (옵션) 충돌 깊이를 반환할 포인터
		_Out_ CCollider::COLLISION_DIR* pOutDir = nullptr	// (옵션) 충돌 방향을 반환할 포인터
	);

	// 특정 그룹 내에서 다중 충돌을 검사하고, 충돌한 객체 목록을 반환합니다.
	_bool Collision_Check_Group_Multi(
		_uint iGroupIndex,											// 검사할 그룹의 인덱스
		list<CCollider_Cube::COLLISION_INFO>& CollisionObjects,		// 충돌한 객체 정보를 저장할 리스트 (출력)
		CComponent* pCollider,										// 충돌을 검사할 대상 Collider
		COLLISION_TYPE eCollisionType								// 충돌할 콜라이더 종류
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