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
	HRESULT CreatePool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, int count = 1, void* pArg = nullptr);

	// �ش� ������Ÿ�� �±� ������Ʈ Ǯ�Ŵ������� ������ ��üȭ.
	// �����ϸ� �߰� ����.
	CGameObject* PushPool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag);

	// �ش� ������Ʈ Ǯ�Ŵ����� ��ȯ.
	void PopPool(CGameObject* _object);

	// �ش� ������Ÿ�� �±� Ǯ�� ���� ��ȯ.
	_uint GetPoolCount(_wstring _tag);

	virtual void Free() override;

private:
	class CGameInstance* m_pGameInstance = { nullptr };	
	
	// Ǯ ��Ų ��ü��.
	unordered_map<_wstring, list<CGameObject*>>* m_mapPoolObjects = {nullptr};
};

END
