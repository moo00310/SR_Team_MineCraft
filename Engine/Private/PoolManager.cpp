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
	// ������Ʈ �Ŵ����� ����.
	CGameObject* obj = m_pGameInstance->Add_GameObjectReturnOBJ(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);	

	// null üũ.
	if (obj == nullptr)
	{
		MSG_BOX("Failed to PoolManager CreatePool !!!");
		return nullptr;
	}	

	// ��Ȱ��ȭ.
	obj->SetActive(false);	
		
	// Ű ã��.
	auto iter = m_mapPoolObjects->find(strPrototypeTag);

	// Ű�� ������.
	if (iter == m_mapPoolObjects->end())
	{
		// ���� �Ҵ�.
		m_mapPoolObjects->insert({ strPrototypeTag, list<CGameObject*>() });

		// ���� �Ҵ��� ���� �ٽ� ã��.
		iter = m_mapPoolObjects->find(strPrototypeTag);
	}

	iter->second.push_back(obj);

	// ��� �Լ� ����.
	if (count <= 1)
	{
		return obj;
	}

	// ���.
	CreatePool(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, --count, pArg);
}

CGameObject* CPoolManager::Push(const _wstring& strPrototypeTag)
{	
	// Ű ã��.
	auto iter = m_mapPoolObjects->find(strPrototypeTag);

	// Ű�� ������.
	if (iter == m_mapPoolObjects->end())
	{
		// ����.
		return nullptr;
	}

	// Ǯ���� �����ð� ��ȸ��.
	for (auto& object : iter->second)
	{
		// ��Ȱ��ȭ �� ������Ʈ ã��.
		if (object->GetActive() == false)
		{
			// �� ���� ��ȯ��.
			object->SetActive(true);					
			return object;
		}
	}

	// �̹� Ǯ�� ����á�ٸ� �߰��� �ϳ� �� �����ϰ� ��ȯ.	
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
