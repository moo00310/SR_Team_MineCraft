#pragma once
#include "Client_Defines.h"
#include "SlotInfo.h"
#include "MainInventory.h"
#include "SubInventory.h"
#include "Inventory_Back.h"
#include "PlayerHP.h"
#include "PlayerHunger.h"
#include "PlayerExp.h"
#include "PlayerLevel.h"
#include "Mouse_Item.h"
#include "Steve.h"
#include "Mouse_ItemFont.h"
#include "CheckBox.h"

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
	void Add_SlotUpdate(CSlotInfo* pItem);
	/* �����԰� �κ��丮 ����ȭ*/
	void Synchronize_Slots();
	/* �÷��̾� ü�� */
	void SetHP();
	/* �÷��̾� �����*/
	void PlayerHunger_Set(_float fTimeDelta);
	/* �÷��̾� ����ġ*/
	void PlayerExp_Set();
	/* �÷��̾� ������*/
	void LevelUp();
	/* ������ ���� ������Ʈ*/
	void ItemCount_Update(ITEMNAME _ItemNamem, _int AddCount);

	ITEMNAME GetItemTypeName();

public:
	CSlotInfo*						Get_Item(int slotIndex);

	vector<CPlayerHP*>*				Get_vecPlayerHPlist(void)		{ return &m_vecPlayerHPlist; }
	vector<CSlotInfo*>*				Get_vecSlotInfolist(void)		{ return &m_vecSlotInfolist; }
	vector<CCheckBox*>*				Get_vecCheckBoxlist(void)		{ return &m_vecCheckBoxlist; }

	list<CPlayerHunger*>*			Get_PlayerHungerlist(void)		{ return &m_PlayerHungerlist; }
	list<CPlayerExp*>*				Get_PlayerExplist(void)			{ return &m_PlayerExplist; }
	list<CPlayerLevel*>*			Get_PlayerLevellist(void)		{ return &m_PlayerLevellist; }
	list<CMouse_Item*>*				Get_MouseItemlist(void)			{ return &m_MouseItemlist; }
	list<CMouse_ItemFont*>*			Get_MouseItemFontlist(void)		{ return &m_MouseItemFontlist; }

	_bool							Get_PlayerHP_Shader()			{ return m_bPlayerHP_Shader; }
	
public:
	void Set_Steve(CSteve* _Steve) 
	{ 
		pSteve = _Steve;
		Safe_AddRef(pSteve);
	}

	void Set_PlayerHP_Shader(_bool _bPlayerHP_Shader) { m_bPlayerHP_Shader = _bPlayerHP_Shader; }

private:
	ITEMID	m_ItemID = ITEMID_END;
	_float	m_fHungerTime = { 0.f };
	_int	m_iallZeroCount = { 0 };
	_int	m_iHP_ChangeCount = { 0 };
	_bool	m_bPlayerHP_Shader = { false };

private:
	/* Player State ���� */
	vector<CPlayerHP*>			m_vecPlayerHPlist;
	list<CPlayerHunger*>		m_PlayerHungerlist;
	list<CPlayerExp*>			m_PlayerExplist;
	list<CPlayerLevel*>			m_PlayerLevellist;

	/* Inventory ���� */
	vector<CSlotInfo*>		m_vecSlotInfolist;
	vector<CCheckBox*>		m_vecCheckBoxlist;
	list<CMainInventory*>	m_MainInventorylist;
	list<CSubInventory*>	m_SubInventorylist;
	list<CInventory_Back*>	m_InventoryBacklist;
	list<CMouse_Item*>		m_MouseItemlist;
	list<CMouse_ItemFont*>	m_MouseItemFontlist;
	
private:
	CSteve* pSteve = { nullptr };

public:
	virtual void Free();
};

END
