#pragma once
#include "Base.h"
#include "GameInstance.h"

BEGIN(Engine)

class CPoolManager final : public CBase
{
private:
	CPoolManager();
	virtual ~CPoolManager() = default;

public:
	HRESULT Initialize();
	static CPoolManager* Create();

	// �ش� ������Ÿ�� �±� ��üȭ �� �� ��Ȱ��ȭ�ؼ� Ǯ�� ���ѵ�.
	CGameObject* CreatePool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, int count = 1, void* pArg = nullptr);

	// �ش� ������Ÿ�� �±� ������Ʈ Ǯ�Ŵ������� ������ ��üȭ.
	// �����ϸ� �߰� ����.
	CGameObject* Push(const _wstring& strPrototypeTag);

	// �ش� ������Ʈ Ǯ�Ŵ����� ��ȯ.
	void Pop(CGameObject* _object, const _wstring& strPrototypeTag);

	virtual void Free() override;

private:
	class CGameInstance* m_pGameInstance = { nullptr };	
	
	// Ǯ ��Ų ��ü��.
	unordered_map<_wstring, list<CGameObject*>>* m_mapPoolObjects = {nullptr};
};

END
