#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

CGameObject::CGameObject(const CGameObject& Prototype) 
	: m_pGraphic_Device{ Prototype.m_pGraphic_Device }
	, m_pGameInstance{ Prototype.m_pGameInstance }
	, m_isCloned{ true }
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	/*if (m_Components.end() != m_Components.find(strComponentTag))
		return E_FAIL;*/

	CComponent*		pComponent = dynamic_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT, iLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CGameObject::Delete_Component(const _wstring& strComponentTag, CComponent* pComponent)
{
	auto range = m_Components.equal_range(strComponentTag); // 해당 태그의 모든 요소 찾기

	for (auto iter = range.first; iter != range.second; ++iter)
	{
		if (iter->second == pComponent) // 포인터 비교
		{
			Safe_Release(iter->second); // 안전하게 해제
			m_Components.erase(iter);   // multimap에서 해당 요소 삭제
			return S_OK; // multimap에서 같은 태그를 가진 요소가 여러 개 있을 수 있으므로, 하나만 삭제 후 종료
		}
	}

	return E_FAIL;
}


CComponent* CGameObject::Find_Component(const _tchar* pComponentTag)
{
	auto	iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponentTag));
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

list<CComponent*> CGameObject::Find_Component_All(const _tchar* pComponentTag)
{
	list<CComponent*> Components;

	for (const auto& pair : m_Components)
	{
		if (pair.first == pComponentTag)
		{
			Components.push_back(pair.second);
		}
	} 

	return Components;
}

CGameInstance* CGameObject::Get_GameInstance()
{
	return m_pGameInstance;
}

_bool CGameObject::Get_isDestroy()
{
	return m_isDestroyed;
}

void CGameObject::Destroy()
{
	m_isDestroyed = true;
}

void CGameObject::SetActive(_bool isActive)
{
	m_isActive = isActive;
}

_bool CGameObject::GetActive() const
{
	return m_isActive;
}

void CGameObject::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();


	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphic_Device);
}
