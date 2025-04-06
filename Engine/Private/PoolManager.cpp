#include "PoolManager.h"
#include "GameObject.h"


// Ǯ���� ��ü ����ϸ�
// Ȱ��ȭ/��Ȱ��ȭ ���ο� ���� Ǯ����.
// CreatePool�� ����ϰ� PushPool�� ������ �� ��ü�� ��Ȱ��ȭ ��Ű�⸸�ϸ� ��.
// PopPool �Լ��� ������ ���� �� �ᵵ �ǰ� SetActive�� false �ɾ ��.
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

HRESULT CPoolManager::CreatePool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, int count, void* pArg)
{	
	// ��� �Լ� ����.
	if (count <= 0)
	{
		return S_OK;
	}

	// ������Ʈ �Ŵ����� ����.
	CGameObject* obj = m_pGameInstance->Add_GameObjectReturnOBJ(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);	

	// null üũ.
	if (obj == nullptr)
	{
		MSG_BOX("Failed to PoolManager CreatePool !!!");
		return E_FAIL;
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

	// m_mapPoolObjects�� �ش� ��ü�� Ǯ�� ��ü�� ���.
	iter->second.push_back(obj);	

	// ���.
	CreatePool(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, --count, pArg);

	return S_OK;
}

CGameObject* CPoolManager::PushPool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag)
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
			// init ���ְ� Ȱ��ȭ.
			object->OnPushPool();
			object->SetActive(true);	
			m_mapActiveCount[strLayerTag]++;

			// �� ���� ��ȯ��.
			return object;
		}
	}

	// �̹� Ǯ�� ����á�ٸ� �߰��� �ϳ� �� �����ϰ� ��ȯ.	
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

void CPoolManager::PopPool(CGameObject* _object, const _wstring& strPrototypeTag)
{
	_object->SetActive(false);
	m_mapActiveCount[strPrototypeTag]--;
}

_uint CPoolManager::GetPoolCount(_wstring _tag)
{
	auto poolObject = m_mapPoolObjects->find(_tag);
	if (m_mapPoolObjects->end() != poolObject)
	{
		// Ű�� ������.
		return static_cast<_uint>(poolObject->second.size());
	}

	// Ű�� ������ 0 ����.
	return 0;
}

void CPoolManager::Free()
{
	__super::Free();	
		
	Safe_Delete(m_mapPoolObjects);

	// m_mapPoolObjects�� ��ȸ�ϸ鼭 �������� �ʴ� ������
	// m_mapPoolObjects�� ������Ʈ �Ŵ����� �����ؼ� ����
	// ������Ʈ �Ŵ����� �̹� ������Ű�� ���⼭�� �����ϸ�
	// ���� ���� ������.

	Safe_Release(m_pGameInstance);
}
