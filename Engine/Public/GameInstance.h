#pragma once

/* 엔진과 클라이언트 간의 링크의 역활을 수행한다. */
/* 엔진 내에 존재하는 유일한 싱글톤 클래스이다. */
/* 엔진 개발자가 클라개T자에게 보여주고싶은 함수를 ... */
#include "Renderer.h"
#include "Prototype_Manager.h"
#include "Collider_Manager.h"
#include "Key_Manager.h"

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
	CGameObject* Add_GameObjectReturnOBJ(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	class CGameObject* Get_Object(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex);
	class CGameObject* Get_LastObject(_uint iLevelIndex, const _tchar* pLayerTag);
	void ClearLayer(_uint iLevelIndex, const _tchar* pLayerTag);
	list<class CGameObject*> Get_GameObjectList(_uint iLevelIndex, const _tchar* pLayerTag);
	// 청크 활성화 관련 (true면 청크 활성화, false면 청크 비활성화 느낌)
	void SetLayerRenderActive(_uint iLevelIndex, const _tchar* pLayerTag, bool _b);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion

#pragma region TIMER_MANAGER
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT Add_Timer(const _wstring& strTimerTag);
	void Update_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region COLLIDER_MANAGER
	//게임오브젝트가 가진 모든 콜라이더를 넣는 함수
	HRESULT Add_CollisionGroup(_uint eCollisionGroup, class CGameObject* pGameObject);
	//콜라이더 하나만 넣는 함수
	HRESULT Add_Collider_CollisionGroup(_uint iGroupIndex, class CCollider* pCollider);

	//콜리전 그룹에서 빼기
	void Out_CollisiomGroup(_uint eCollisionGroup, class CGameObject* pGameObject);
	//특정 그룹의 오브젝트들과 충돌 검사
	class CGameObject* Collision_Check_with_Group(_uint eGroup, CComponent* pCollider, CCollider_Manager::COLLISION_TYPE eType, _Out_ _float3* pOutDepth = nullptr, _Out_ CCollider_Cube::COLLISION_DIR* pOutDir = nullptr);
	//특정 그룹의 오브젝트들과 충돌 검사(여러개의 오브젝트와 충돌 검사)
	_bool Collision_Check_Group_Multi(_uint iGroupIndex, list<CCollider_Cube::COLLISION_INFO>& CollisionObjects, CComponent* pCollider, CCollider_Manager::COLLISION_TYPE eCollisionType);
	
	//일반 레이케스트
	CGameObject* Ray_Cast(const _float3& vRayOrigin, const _float3& vRayDir, _float fMaxDistance, _uint iGroup, _Out_ _float* pDist = nullptr);
	//인스턴싱 오브젝트 레이케스트
	CGameObject* Ray_Cast_InstancedObjects(const _float3& vRayOrigin, const _float3& vRayDir, _float fMaxDistance, _uint iGroup, _Out_ _float* pDist = nullptr, _Out_ _float3* pOutCollision_Dir = nullptr, _Out_ CComponent** ppOutCollider = nullptr);
	//인스턴싱 오브젝트 레이케스트(여러개의 그룹과 충돌 검사)
	CGameObject* Ray_Cast_MultiGroup_InstancedObjects(const _float3& rayOrigin, const _float3& rayDir, _float fMaxDistanc, const ::vector<_uint>& vGroupIndices, _Out_ _float* pDist = nullptr, _Out_ _float3* pOutCollision_Dir = nullptr, _Out_ CComponent** ppOutCollider = nullptr);
#pragma endregion

#pragma region FRUSTUM_CULLING_MANAGER
	_bool   Is_In_Frustum(_float3 vPos, _float fRadius);
#pragma endregion


#pragma region KEY_MANAGER
	_bool		Key_Pressing(int _Key);
	_bool		Key_Up(int _Key);		// 누르고 있다가 뗐을 때
	_bool		Key_Down(int _Key);		// 눌렀을 때
#pragma endregion

#pragma region SOUND_MANAGER
	//void		Play_Sound(const char* _EventPath);
#pragma endregion


#pragma region POOL_MANAGER

	// 풀링 객체 생성.
	HRESULT CreatePool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, int count = 1, void* pArg = nullptr);

	// 풀링 객체 꺼내옴.
	CGameObject* PushPool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag);

	// 풀링으로 돌아감.
	void PopPool(CGameObject* _object);

	// 해당 프로토타입 태그 풀링 객체 갯수 반환.
	int GetPoolCount(_wstring _tag);
	
#pragma endregion

private:
	class CGraphic_Device*				m_pGraphic_Device = { nullptr };
	class CLevel_Manager*				m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*			m_pPrototype_Manager = { nullptr };
	class CObject_Manager*				m_pObject_Manager = { nullptr };
	class CRenderer*					m_pRenderer = { nullptr };
	class CTimer_Manager*				m_pTimer_Manager = { nullptr };
	class CCollider_Manager*			m_pCollider_Manager = { nullptr };
	class CFrustumCulling_Manager*		m_pFrustumCulling_Manager = { nullptr };
	class CKey_Manager*					m_pKey_Manager = { nullptr };
	//class CSound_Manager*				m_pSound_Manager = { nullptr };
	class CPoolManager*					m_pPoolManager = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;
};

END