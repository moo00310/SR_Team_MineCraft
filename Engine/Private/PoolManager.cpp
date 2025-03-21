#include "PoolManager.h"
#include "GameObject.h"

CPoolManager::CPoolManager() : 
	m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPoolManager::Initialize()
{
	m_mapPoolObjects = new unordered_map<_wstring, list<CGameObject*>>();

	return S_OK;
}

CPoolManager* CPoolManager::Create()
{
	CPoolManager* pInstance = new CPoolManager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CPoolManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPoolManager::CreatePool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, int count, void* pArg)
{	
	// 오브젝트 매니저에 생성.
	CGameObject* obj = m_pGameInstance->Add_GameObjectReturnOBJ(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);	

	// null 체크.
	if (obj == nullptr)
	{
		MSG_BOX("Failed to PoolManager CreatePool !!!");
		return nullptr;
	}	

	// 비활성화.
	obj->SetActive(false);	
		
	// 키 찾음.
	auto iter = m_mapPoolObjects->find(strPrototypeTag);

	// 키가 없으면.
	if (iter == m_mapPoolObjects->end())
	{
		// 새로 할당.
		m_mapPoolObjects->insert({ strPrototypeTag, list<CGameObject*>() });

		// 새로 할당한 영역 다시 찾음.
		iter = m_mapPoolObjects->find(strPrototypeTag);
	}

	iter->second.push_back(obj);

	// 재귀 함수 종료.
	if (count <= 1)
	{
		return obj;
	}

	// 재귀.
	CreatePool(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, --count, pArg);
}

CGameObject* CPoolManager::Push(const _wstring& strPrototypeTag)
{	
	// 키 찾음.
	auto iter = m_mapPoolObjects->find(strPrototypeTag);

	// 키가 없으면.
	if (iter == m_mapPoolObjects->end())
	{
		// 멈춤.
		return nullptr;
	}

	// 풀에서 꺼내올거 순회함.
	for (auto& object : iter->second)
	{
		// 비활성화 된 오브젝트 찾음.
		if (object->GetActive() == false)
		{
			// 이 새기 반환함.
			object->SetActive(true);					
			return object;
		}
	}

	// 이미 풀에 가득찼다면 추가로 하나 더 생성하고 반환.	
	return CreatePool(0, strPrototypeTag, 0, TEXT("asd"), 1);
}

void CPoolManager::Pop(CGameObject* _object, const _wstring& strPrototypeTag)
{
	_object->SetActive(false);
}

void CPoolManager::Free()
{
	__super::Free();	
		
	Safe_Delete(m_mapPoolObjects);

	Safe_Release(m_pGameInstance);
}
