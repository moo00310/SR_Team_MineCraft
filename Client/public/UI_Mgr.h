#pragma once
#include "Client_Defines.h"
#include "Item.h"
#include "MainInventory.h"
#include "SubInventory.h"
#include "Inventory_Back.h"
#include "PlayerHP.h"
#include "PlayerHunger.h"
#include "PlayerExp.h"
#include "PlayerLevel.h"

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
	/* 퀵슬롯과 인벤토리 동기화*/
	void Synchronize_Slots();
	/* 플레이어 체력 */
	void TakeDamge();
	/* 플레이어 배고픔*/
	void PlayerHunger_Set(_float fTimeDelta);
	/* 플레이어 경험치*/
	void PlayerExp_Set();
	/* 플레이어 레벨업*/
	void LevelUp();

public:
	CItem*					Get_Item(int slotIndex);

	list<CPlayerHP*>*		Get_PlayerHPlist(void)		{ return &m_PlayerHPlist; }
	list<CPlayerHunger*>*	Get_PlayerHungerlist(void)	{ return &m_PlayerHungerlist; }
	list<CPlayerExp*>*		Get_PlayerExplist(void)		{ return &m_PlayerExplist; }
	list<CPlayerLevel*>* Get_PlayerLevellist(void)		{ return &m_PlayerLevellist; }

private:
	_float	m_fHungerTime = { 0.f };
	_int	m_iallZeroCount = { 0 };

private:
	vector<CItem*>			m_vecItemlist;
	list<CMainInventory*>	m_MainInventorylist;
	list<CSubInventory*>	m_SubInventorylist;
	list<CInventory_Back*>	m_InventoryBacklist;

	/* Player State 관련 */
	list<CPlayerHP*>		m_PlayerHPlist;
	list<CPlayerHunger*>	m_PlayerHungerlist;
	list<CPlayerExp*>		m_PlayerExplist;
	list<CPlayerLevel*>		m_PlayerLevellist;

public:
	virtual void Free();
};

END
