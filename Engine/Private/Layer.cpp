#include "Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end();)
	{
		auto pGameObject = *iter;

		// ���� ��ü�� �ı��Ǿ����� �����ϰ� �����ϰ� ����
		if (pGameObject->Get_isDestroy())
		{
			Safe_Release(pGameObject);
			iter = m_GameObjects.erase(iter); // ���� �� �ݺ��� ������Ʈ
		}
		else
		{
			pGameObject->Priority_Update(fTimeDelta);
			++iter; // �������� ������ ��� ����
		}
	}

}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Update(fTimeDelta);

	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	if (m_bLayerActive) {
		for (auto& pGameObject : m_GameObjects)
		{
			if (nullptr != pGameObject)
				pGameObject->Late_Update(fTimeDelta);

		}
	}
}

CGameObject* CLayer::Get_Object(_uint iIndex)
{
	if (m_GameObjects.size() <= iIndex)
		return nullptr;

	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return *iter;
}

list<class CGameObject*> CLayer::Get_GameObjectList()
{
	return m_GameObjects;
}

void CLayer::SetLayerRenderActive(bool _b)
{
	m_bLayerActive = _b;
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

 	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);
	m_GameObjects.clear();
}
