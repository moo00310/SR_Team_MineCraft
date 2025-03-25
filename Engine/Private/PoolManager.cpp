#include "PoolManager.h"
#include "GameObject.h"


// 풀링에 객체 등록하면
// 활성화/비활성화 여부에 따라 풀링됨.
// CreatePool로 등록하고 PoolPush로 꺼내서 그 객체를 비활성화 시키기만하면 됨.
// PopPool 함수가 있지만 굳이 안 써도 되고 SetActive로 false 걸어도 됨.
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

void CPoolManager::CreatePool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, int count, void* pArg)
{	
	// 재귀 함수 종료.
	if (count <= 0)
	{
		return;
	}

	// 오브젝트 매니저에 생성.
	CGameObject* obj = m_pGameInstance->Add_GameObjectReturnOBJ(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);	

	// null 체크.
	if (obj == nullptr)
	{
		MSG_BOX("Failed to PoolManager CreatePool !!!");
		return;
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

	// m_mapPoolObjects에 해당 객체를 풀링 객체로 등록.
	iter->second.push_back(obj);	

	// 재귀.
	CreatePool(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, --count, pArg);
}

CGameObject* CPoolManager::PushPool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag)
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
			// init 해주고 활성화.
			object->Initialize(nullptr);
			object->SetActive(true);					

			// 이 새기 반환함.
			return object;
		}
	}

	// 이미 풀에 가득찼다면 추가로 하나 더 생성하고 반환.	
	CreatePool(iPrototypeLevelIndex,
		strPrototypeTag,
		iLevelIndex,
		strLayerTag,
		1);

	return PushPool(iPrototypeLevelIndex,
		strPrototypeTag,
		iLevelIndex,
		strLayerTag);
}

void CPoolManager::PopPool(CGameObject* _object)
{
	_object->SetActive(false);
}

void CPoolManager::Free()
{
	__super::Free();	
		
	Safe_Delete(m_mapPoolObjects);

	// m_mapPoolObjects를 순회하면서 삭제하지 않는 이유는
	// m_mapPoolObjects는 오브젝트 매니저랑 공유해서 쓴다
	// 오브젝트 매니저가 이미 삭제시키니 여기서도 삭제하면
	// 이중 해제 오류뜸.

	Safe_Release(m_pGameInstance);
}
