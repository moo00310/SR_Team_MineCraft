#pragma once

/* ������ Ŭ���̾�Ʈ ���� ��ũ�� ��Ȱ�� �����Ѵ�. */
/* ���� ���� �����ϴ� ������ �̱��� Ŭ�����̴�. */
/* ���� �����ڰ� Ŭ�󰳓T�ڿ��� �����ְ���� �Լ��� ... */
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
	void ClearLayer(_uint iLevelIndex, const _tchar* pLayerTag);
	list<class CGameObject*> Get_GameObjectList(_uint iLevelIndex, const _tchar* pLayerTag);
	// ûũ Ȱ��ȭ ���� (true�� ûũ Ȱ��ȭ, false�� ûũ ��Ȱ��ȭ ����)
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
	//�ݸ��� �׷쿡 �߰�
	HRESULT Add_CollisionGroup(_uint eCollisionGroup, class CGameObject* pGameObject);
	//�ݸ��� �׷쿡�� ����
	void Out_CollisiomGroup(_uint eCollisionGroup, class CGameObject* pGameObject);
	//Ư�� �׷��� ������Ʈ��� �浹 �˻�
	_bool Collision_with_Group(_uint eGroup, CComponent* pCollider, CCollider_Manager::COLLISION_TYPE eType, _Out_ _float3* pOutDistance = nullptr, _Out_ CCollider_Cube::COLLSION_DIR* pOutDir = nullptr);

	//�����ɽ�Ʈ
	//_bool Ray_Cast(const _float4x4* matWorld, _float3 vOrigin, _float3 vDir, _float fLength, CCollider_Manager::COLLISION_GROUP eGroup);

	_bool Ray_Cast(const _float3& rayOrigin, const _float3& rayDir, _float maxDistance, _uint iGroup, _Out_ _float& fDist, _Out_ CGameObject** ppGameObject = nullptr);
#pragma endregion

#pragma region KEY_MANAGER
	_bool		Key_Pressing(int _Key);
	_bool		Key_Up(int _Key);		// ������ �ִٰ� ���� ��
	_bool		Key_Down(int _Key);		// ������ ��
#pragma endregion

#pragma region POOL_MANAGER

	// Ǯ�� ��ü ����.
	void CreatePool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, int count = 1, void* pArg = nullptr);

	// Ǯ�� ��ü ������.
	CGameObject* Push(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag);

	// Ǯ������ ���ư�.
	void Pop(CGameObject* _object);
	
#pragma endregion

private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*	m_pPrototype_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };
	class CCollider_Manager*	m_pCollider_Manager = { nullptr };
	class CKey_Manager*			m_pKey_Manager = { nullptr };
	class CPoolManager*			m_pPoolManager = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;
};

END