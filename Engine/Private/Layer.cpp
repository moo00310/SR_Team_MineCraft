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

		// 게임 객체가 파괴되었으면 안전하게 해제하고 삭제
		if (pGameObject->Get_isDestroy())
		{
			Safe_Release(pGameObject);
			iter = m_GameObjects.erase(iter); // 삭제 후 반복자 업데이트
		}
		else
		{
			pGameObject->Priority_Update(fTimeDelta);
			++iter; // 삭제되지 않으면 계속 진행
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
