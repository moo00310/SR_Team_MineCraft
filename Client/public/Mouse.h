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
	bool		Get_Picked(void)	{ return m_bPicked; }
	ITEMNAME	Get_ItemName(void)	{ return m_ItemName; }
	ITEMID		Get_ItemID(void)	{ return m_ItemID; }
	_int		Get_SlotIndex(_int) { return m_iSlotIndex; }
	_int		Get_ItemCount() { return m_iItemCount; }

public:
	void Set_Picked(bool _bPicked)			{ m_bPicked = _bPicked; }
	void Set_ItemName(ITEMNAME _ItemName)	{ m_ItemName = _ItemName; }
	void Set_ItemID(ITEMID _ItemID)			{ m_ItemID = _ItemID; }
	void Set_SlotIndex(_int _iSlotIndex)	{ m_iSlotIndex = _iSlotIndex; }
	void Set_ItemCount(_int _itemCount) { m_iItemCount = _itemCount; }

public:
	void Clear(_uint iLevelIndex);

private:
	ITEMNAME	m_ItemName = ITEMNAME_END;
	ITEMID		m_ItemID = ITEMID_END;


	_int	m_iSlotIndex = { 0 };
	_int	m_iItemCount = { 0 };
	bool	m_bPicked = { false };


public:
	virtual void Free() override;
};

END
