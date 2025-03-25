#pragma once

#include "Base.h"

/* ��ü���� ��Ƴ��´�. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	CGameObject* Get_Object(_uint iIndex);
	list<class CGameObject*> Get_GameObjectList();
	CGameObject* Get_LastObject();
	void SetLayerRenderActive(bool _b);
private:
	list<class CGameObject*>			m_GameObjects;
	bool m_bLayerActive = true;

public:
	static CLayer* Create();
	virtual void Free();
};

END