#include "GameInstance.h"

#include "Renderer.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Prototype_Manager.h"
#include "Collider_Manager.h"

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

	m_pCollider_Manager = CCollider_Manager::Create(*ppOut);
	if (nullptr == m_pCollider_Manager)
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
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

CGameObject* CGameInstance::Get_Object(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_Objects(iLevelIndex, pLayerTag, iIndex);
}

void CGameInstance::ClearLayer(_uint iLevelIndex, const _tchar* pLayerTag)
{
	m_pObject_Manager->ClearLayer(iLevelIndex, pLayerTag);
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

HRESULT CGameInstance::Add_CollisionGroup(CCollider_Manager::COLLISION_GROUP eCollisionGroup, CGameObject* pGameObject)
{
	if (!m_pCollider_Manager)
		return E_FAIL;

	return m_pCollider_Manager->Add_CollisionGroup(eCollisionGroup, pGameObject);
}

void CGameInstance::Out_CollisiomGroup(CCollider_Manager::COLLISION_GROUP eCollisionGroup, CGameObject* pGameObject)
{
	if (!m_pCollider_Manager)
		return;

	m_pCollider_Manager->Out_CollisiomGroup(eCollisionGroup, pGameObject);
}

_bool CGameInstance::Collision_with_Group(CCollider_Manager::COLLISION_GROUP eGroup, CGameObject* pGameObject, CCollider_Manager::COLLISION_TYPE eType, _float3* pOutDistance)
{
	if (nullptr == m_pCollider_Manager)
		return false;

	return m_pCollider_Manager->Collision_with_Group(eGroup, pGameObject, eType, pOutDistance);
}

_bool CGameInstance::Ray_Cast(const _float4x4* matWorld, _float3 vOrigin, _float3 vDir, _float fLength, CCollider_Manager::COLLISION_GROUP eGroup)
{
	if (nullptr == m_pCollider_Manager)
		return false;

	return m_pCollider_Manager->Ray_Cast(matWorld, vOrigin, vDir, fLength, eGroup);
}

_bool CGameInstance::IntersectRayOBB(const _float3& rayOrigin, const _float3& rayDir, float& tMin, CCollider_Manager::COLLISION_GROUP eGroup)
{
	if (nullptr == m_pCollider_Manager)
		return false;

	return m_pCollider_Manager->IntersectRayOBB(rayOrigin, rayDir, tMin, eGroup);
}

#pragma endregion

void CGameInstance::Release_Engine()
{
	Safe_Release(m_pTimer_Manager);

	Safe_Release(m_pRenderer);

	Safe_Release(m_pObject_Manager);

	Safe_Release(m_pPrototype_Manager);

	Safe_Release(m_pLevel_Manager);

	Safe_Release(m_pCollider_Manager);

	Safe_Release(m_pGraphic_Device);

	Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();

	
}
