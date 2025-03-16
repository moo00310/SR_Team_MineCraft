#pragma once

/* 엔진과 클라이언트 간의 링크의 역활을 수행한다. */
/* 엔진 내에 존재하는 유일한 싱글톤 클래스이다. */
/* 엔진 개발자가 클라개밫자에게 보여주고싶은 함수를 ... */
#include "Renderer.h"
#include "Prototype_Manager.h"

#include "Collider_Manager.h" //콜라이더 그룹 enum 가져오려고 인클루드함...클라에서 enum 관리가 맞는 것 같음

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ LPDIRECT3DDEVICE9* ppOut);
	void Update_Engine(_float fTimeDelta);
	HRESULT Draw();	
	void Clear(_uint iLevelIndex);

#pragma region LEVEL_MANAGER
public:
	HRESULT Change_Level(_uint iLevelIndex, class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	class CGameObject* Get_Object(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex);
	void ClearLayer(_uint iLevelIndex, const _tchar* pLayerTag);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion

#pragma region TIMER_MANAGER
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT Add_Timer(const _wstring& strTimerTag);
	void Update_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region Collider_Manager
	//콜리전 그룹에 추가
	HRESULT Add_CollisionGroup(_uint eCollisionGroup, class CGameObject* pGameObject);
	//콜리전 그룹에서 빼기
	void Out_CollisiomGroup(_uint eCollisionGroup, class CGameObject* pGameObject);
	//특정 그룹의 오브젝트들과 충돌 검사
	_bool Collision_with_Group(_uint eGroup, class CGameObject* pGameObject, CCollider_Manager::COLLISION_TYPE eType, _float3* pOutDistance = nullptr);

	//레이케스트
	//_bool Ray_Cast(const _float4x4* matWorld, _float3 vOrigin, _float3 vDir, _float fLength, CCollider_Manager::COLLISION_GROUP eGroup);

	_bool Ray_Cast(const _float3& rayOrigin, const _float3& rayDir, _float maxDistance, _uint iGroup, _Out_ _float& fDist);
#pragma endregion


private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*	m_pPrototype_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };
	class CCollider_Manager*	m_pCollider_Manager = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;
};

END