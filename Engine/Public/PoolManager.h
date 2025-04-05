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

	// 해당 프로토타입 태그 객체화 한 후 비활성화해서 풀링 시켜둠.
	HRESULT CreatePool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, int count = 1, void* pArg = nullptr);

	// 해당 프로토타입 태그 오브젝트 풀매니저에서 가져와 객체화.
	// 부족하면 추가 생성.
	CGameObject* PushPool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag);

	// 해당 오브젝트 풀매니저로 반환.
	void PopPool(CGameObject* _object);

	// 해당 프로토타입 태그 풀링 갯수 반환.
	_uint GetPoolCount(_wstring _tag);

	virtual void Free() override;

private:
	class CGameInstance* m_pGameInstance = { nullptr };	
	
	// 풀 시킨 객체들.
	unordered_map<_wstring, list<CGameObject*>>* m_mapPoolObjects = {nullptr};
};

END
