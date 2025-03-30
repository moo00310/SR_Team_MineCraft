#include "GameInstance.h"

#include "Renderer.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Prototype_Manager.h"
#include "Collider_Manager.h"
//#include "Sound_Manager.h"
#include "PoolManager.h"

IMPLEMENT_SINGLETON(CGameInstance);

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ LPDIRECT3DDEVICE9* ppOut)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pCollider_Manager = CCollider_Manager::Create(*ppOut, EngineDesc.iNumCollisionGroups);
	if (nullptr == m_pCollider_Manager)
		return E_FAIL;

	m_pKey_Manager = CKey_Manager::Create();
	if (nullptr == m_pKey_Manager)
		return E_FAIL;

	/*m_pSound_Manager = CSound_Manager::Create();
	if (nullptr == m_pSound_Manager)
		return E_FAIL;*/

	m_pPoolManager = CPoolManager::Create();
	if (nullptr == m_pPoolManager)
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pKey_Manager->Update();
	//m_pSound_Manager->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pObject_Manager->Update(fTimeDelta);

	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);

	m_pCollider_Manager->Reset_ColliderGroup(); // 마지막에 호출하라고함 (그룹에 넣다 뺏다 반복 하는 듯)
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Render_Begin();

	m_pRenderer->Draw();

	m_pCollider_Manager->Render();

	m_pLevel_Manager->Render();

	m_pGraphic_Device->Render_End();

	return S_OK;
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	/* 특정 레벨의 자원을 삭제한다. */
	
	/* 특정 레벨의 객체을 삭제한다. */
	m_pObject_Manager->Clear(iLevelIndex);

	/* 특정 레벨의 원형객을 삭제한다. */
	m_pPrototype_Manager->Clear(iLevelIndex);
}

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Change_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(iLevelIndex, pNewLevel);
}
#pragma endregion

#pragma region PROTOTYPE_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iPrototypeLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototypeType, iPrototypeLevelIndex, strPrototypeTag, pArg);
}
#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}

CGameObject* CGameInstance::Add_GameObjectReturnOBJ(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObjectReturnOBJ(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}

CGameObject* CGameInstance::Get_Object(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_Objects(iLevelIndex, pLayerTag, iIndex);
}

CGameObject* CGameInstance::Get_LastObject(_uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;
	return m_pObject_Manager->Find_LastObject(iLevelIndex, pLayerTag);
}

void CGameInstance::ClearLayer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	m_pObject_Manager->ClearLayer(iLevelIndex, pLayerTag);
}

list<class CGameObject*> CGameInstance::Get_GameObjectList(_uint iLevelIndex, const _tchar* pLayerTag)
{
	return m_pObject_Manager->Get_GameObjectList(iLevelIndex, pLayerTag);
}

void CGameInstance::SetLayerRenderActive(_uint iLevelIndex, const _tchar* pLayerTag, bool _b)
{
	m_pObject_Manager->SetLayerRenderActive(iLevelIndex, pLayerTag, _b);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

#pragma endregion

#pragma region TIMER_MANAGER

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Update_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Update(strTimerTag);
}
#pragma endregion

#pragma region COLLIDER_MANAGER
HRESULT CGameInstance::Add_CollisionGroup(_uint eCollisionGroup, CGameObject* pGameObject)
{
	if (!m_pCollider_Manager)
		return E_FAIL;

	return m_pCollider_Manager->Add_CollisionGroup(eCollisionGroup, pGameObject);
}

HRESULT CGameInstance::Add_Collider_CollisionGroup(_uint iGroupIndex, CCollider* pCollider)
{
	if (!m_pCollider_Manager)
		return E_FAIL;

	return m_pCollider_Manager->Add_Collider_CollisionGroup(iGroupIndex, pCollider);
}

void CGameInstance::Out_CollisiomGroup(_uint eCollisionGroup, CGameObject* pGameObject)
{
	if (!m_pCollider_Manager)
		return;

	m_pCollider_Manager->Out_CollisiomGroup(eCollisionGroup, pGameObject);
}

CGameObject* CGameInstance::Collision_Check_with_Group(_uint eGroup, CComponent* pCollider, CCollider_Manager::COLLISION_TYPE eType, _Out_ _float3* pOutDepth, _Out_ CCollider_Cube::COLLISION_DIR* pOutDir)
{
	if (pOutDepth) *pOutDepth = { 0.f, 0.f, 0.f };
	if (pOutDir) *pOutDir = CCollider::COLLISION_DIR::NONE;

	if (nullptr == m_pCollider_Manager)
		return nullptr;

	return m_pCollider_Manager->Collision_Check_with_Group(eGroup, pCollider, eType, pOutDepth, pOutDir);
}

_bool CGameInstance::Collision_Check_Group_Multi(_uint iGroupIndex, list<CCollider_Cube::COLLISION_INFO>& Collision_Infos, CComponent* pCollider, CCollider_Manager::COLLISION_TYPE eCollisionType)
{
	if (nullptr == m_pCollider_Manager)
		return false;

	return m_pCollider_Manager->Collision_Check_Group_Multi(iGroupIndex, Collision_Infos, pCollider, eCollisionType);
}

CGameObject* CGameInstance::Ray_Cast(const _float3& vRayOrigin, const _float3& vRayDir, _float fMaxDistance, _uint iGroup, _Out_ _float* pDist)
{
	if (pDist) *pDist = 0.f;

	if (nullptr == m_pCollider_Manager)
		return nullptr;

	return m_pCollider_Manager->Ray_Cast(vRayOrigin, vRayDir, fMaxDistance, iGroup, pDist);
}

CGameObject* CGameInstance::Ray_Cast_InstancedObjects(const _float3& vRayOrigin, const _float3& vRayDir, _float fMaxDistance, _uint iGroup, _Out_ _float* pDist, _Out_ _float3* pOutCollision_Dir, _Out_ CComponent** ppOutCollider)
{
	if (nullptr == m_pCollider_Manager)
		return nullptr;

	if (pDist) *pDist = 0.f;
	if (pOutCollision_Dir) *pOutCollision_Dir = { 0.f, 0.f, 0.f };
	if (ppOutCollider) *ppOutCollider = nullptr;

	return m_pCollider_Manager->Ray_Cast_InstancedObjects(vRayOrigin, vRayDir, fMaxDistance, iGroup, pDist, pOutCollision_Dir, ppOutCollider);
}
CGameObject* CGameInstance::Ray_Cast_MultiGroup_InstancedObjects(const _float3& rayOrigin, const _float3& rayDir, _float fMaxDistanc, const::vector<_uint>& vGroupIndices, _Out_ _float* pDist, _Out_ _float3* pOutCollision_Dir, _Out_ CComponent** ppOutCollider)
{
	if (nullptr == m_pCollider_Manager)
		return nullptr;

	if (pDist) *pDist = 0.f;
	if (pOutCollision_Dir) *pOutCollision_Dir = { 0.f, 0.f, 0.f };
	if (ppOutCollider) *ppOutCollider = nullptr;

	return m_pCollider_Manager->Ray_Cast_MultiGroup_InstancedObjects(rayOrigin, rayDir, fMaxDistanc, vGroupIndices, pDist, pOutCollision_Dir, ppOutCollider);
}
#pragma endregion

#pragma region KEY_MANAGER
_bool CGameInstance::Key_Pressing(int _Key)
{
	return m_pKey_Manager->Key_Pressing(_Key);
}

_bool CGameInstance::Key_Up(int _Key)
{
	return m_pKey_Manager->Key_Up(_Key);
}

_bool CGameInstance::Key_Down(int _Key)
{
	return m_pKey_Manager->Key_Down(_Key);
}
//void CGameInstance::Play_Sound(const char* _EventPath)
//{
//	m_pSound_Manager->PlayEvent(_EventPath);
//}
#pragma endregion

#pragma region POOL_MANAGER
void CGameInstance::CreatePool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, int count, void* pArg)
{
	m_pPoolManager->CreatePool(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, count, pArg);
}
CGameObject* CGameInstance::PushPool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pPoolManager->PushPool(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag);
}
void CGameInstance::PopPool(CGameObject* _object)
{
	m_pPoolManager->PopPool(_object);
}
int CGameInstance::GetPoolCount(_wstring _tag)
{
	return m_pPoolManager->GetPoolCount(_tag);
}
#pragma endregion

void CGameInstance::Release_Engine()
{
	//Safe_Release(m_pSound_Manager);

	Safe_Release(m_pKey_Manager);

	Safe_Release(m_pTimer_Manager);

	Safe_Release(m_pRenderer);

	Safe_Release(m_pObject_Manager);

	Safe_Release(m_pPrototype_Manager);

	Safe_Release(m_pLevel_Manager);

	Safe_Release(m_pCollider_Manager);

	Safe_Release(m_pPoolManager);

	Safe_Release(m_pGraphic_Device);

	Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();

	
}
