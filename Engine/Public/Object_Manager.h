#pragma once

#include "Base.h"

/* 레벨별로 구분하여 사본객체를 보관한다. */
/* 리스트들로 보관한다. 리스트 == 객체들 == 내가 나누고 싶은 기준에따라 그룹짓는다. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg);

	// 반환 타입이 생성한 객체임.
	class CGameObject* Add_GameObjectReturnOBJ(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg);

	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Clear(_uint iLevelIndex);
	void ClearLayer(_uint iLevelIndex, const _tchar* pLayerTag);
	class CGameObject* Find_Objects(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex);
	class CGameObject* Find_LastObject(_uint iLevelIndex, const _tchar* pLayerTag);
	list<class CGameObject*> Get_GameObjectList(_uint iLevelIndex, const _tchar* pLayerTag);
	void SetLayerRenderActive(_uint iLevelIndex, const _tchar* pLayerTag, bool _b);
private:	
	_uint								m_iNumLevels = {};
	map<const _wstring, class CLayer*>* m_pLayers = { nullptr };
	class CGameInstance*				m_pGameInstance = { nullptr };

private:
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END