#pragma once
#include "Client_Defines.h"
#include "Item.h"
#include "MainInventory.h"
#include "SubInventory.h"
#include "Inventory_Back.h"


class CInventory_Mgr final : public CBase
{
	DECLARE_SINGLETON(CInventory_Mgr);

private:
	CInventory_Mgr();
	virtual ~CInventory_Mgr() = default;


public:
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);


public:
	void Synchronize_Slots();
	void Add_Item(CItem* pItem);
	CItem* Get_Item(int slotIndex);

public:
	_int		Get_SlotIndex() { return m_iSlotIndex; }
	ITEMNAME	Get_ItemTexture() { return m_ItemTextureNum; }

private:
	_int		m_iSlotIndex;
	ITEMNAME	m_ItemTextureNum = ITEMNAME_END;

private:
	vector<CItem*>			m_vecItemlist;
	list<CMainInventory*>	m_MainInventorylist;
	list<CSubInventory*>	m_SubInventorylist;
	list<CInventory_Back*>	m_InventoryBacklist;

};

