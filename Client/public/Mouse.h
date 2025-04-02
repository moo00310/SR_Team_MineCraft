#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CMouse final : public CBase
{
	DECLARE_SINGLETON(CMouse);

private:
	CMouse();
	virtual ~CMouse() = default;

public:
	typedef struct tagItemDesc
	{
		_float  fSizeX, fSizeY;
		_float	fX, fY;
	}ITEM_DESC;

public:
	ITEMNAME	Get_ItemName(void)			{ return m_ItemName; }
	ITEMNAME	Get_OldItemName(void)		{ return m_OldItem; }
	ITEMNAME	Get_UpdateItemName(void)	{ return m_OldItem; }
	ITEMID		Get_ItemID(void)			{ return m_ItemID; }
	_bool		Get_Picked(void)			{ return m_bPicked; }
	_bool		Get_ItemMatch()				{ return m_bItemMatch; }
	_int		Get_SlotIndex()				{ return m_iSlotIndex; }
	_int		Get_ItemCount()				{ return m_iItemCount; }
	_int		Get_OldItemCount()			{ return m_iOldItemCount; }


public:
	void Set_ItemName(ITEMNAME _ItemName)		{ m_ItemName = _ItemName; }
	void Set_OldItem(ITEMNAME _OldItem)			{ m_OldItem = _OldItem; }
	void Set_UpdateItem(ITEMNAME _UpdateItem)	{ m_UpdateItem = _UpdateItem; }
	void Set_ItemID(ITEMID _ItemID)				{ m_ItemID = _ItemID; }
	void Set_Picked(_bool _bPicked)				{ m_bPicked = _bPicked; }
	void Set_ItemMatch(_bool _ItemMatch)		{ m_bItemMatch = _ItemMatch; }
	void Set_ItemCount(_int _ItemCount)			{ m_iItemCount = _ItemCount; }
	void Set_OldItemCount(_int OldItemCount)	{ m_iOldItemCount = OldItemCount; }
	void Set_SlotIndex(_int _ISlotIndex)		{ m_iSlotIndex = _ISlotIndex; }
	//void Set_ItemPos()

public:
	void Clear(_uint iLevelIndex);

private:
	ITEMNAME	m_ItemName = ITEMNAME_END;
	ITEMNAME	m_OldItem = ITEMNAME_END;
	ITEMNAME	m_UpdateItem = ITEMNAME_END;
	
	ITEMID		m_ItemID = ITEMID_END;

	_int	m_iSlotIndex = { 0 };
	_int	m_iItemCount = { 0 };
	_int	m_iOldItemCount = { 0 };
	_bool	m_bPicked = { false };
	_bool	m_bItemMatch = { false };

public:
	virtual void Free() override;
};

END
