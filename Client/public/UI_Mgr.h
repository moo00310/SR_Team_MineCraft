#pragma once
#include "Client_Defines.h"
#include "Item.h"
#include "MainInventory.h"
#include "SubInventory.h"
#include "Inventory_Back.h"
#include "PlayerHP.h"

BEGIN(Client)

class CUI_Mgr final : public CBase
{
	DECLARE_SINGLETON(CUI_Mgr);

private:
	CUI_Mgr();
	virtual ~CUI_Mgr() = default;

public:
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

public:
	void Add_Item(CItem* pItem);

public:
	CItem* Get_Item(int slotIndex);
	list<CPlayerHP*>* Get_PlayerHPlist(void) { return &m_PlayerHPlist; }

public:
	void Synchronize_Slots();
	void TakeDamge();

private:
	_int		m_iSlotIndex;
	ITEMNAME	m_ItemTextureNum = ITEMNAME_END;

private:
	vector<CItem*>			m_vecItemlist;
	list<CMainInventory*>	m_MainInventorylist;
	list<CSubInventory*>	m_SubInventorylist;
	list<CInventory_Back*>	m_InventoryBacklist;

	list<CPlayerHP*>		m_PlayerHPlist;


public:
	virtual void Free();
};

END
