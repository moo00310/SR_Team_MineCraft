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
#include "Bag.h"
#include "Item.h"
#include "Crafting.h"

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
	/* 퀵슬롯과 인벤토리 동기화*/
	void Synchronize_Slots();
	/* 플레이어 체력 */
	void SetHP();
	/* 플레이어 배고픔*/
	void PlayerHunger_Set(_float fTimeDelta);
	/* 플레이어 경험치*/
	void PlayerExp_Set();
	/* 플레이어 레벨업*/
	void LevelUp(_int _iLevel);
	/* 아이템 개수 업데이트*/
	void ItemCount_Update(ITEMNAME _ItemNamem, _int AddCount);
	/* 아이템 개수 나누기 */
	void Split_ItemStack(_int _SlotIndex, ITEMID _ItemID, ITEMNAME _ItemName);
	/* 화로, 제작대, 인벤토리 활성화 시 비활성되는 슬롯 관리*/
	void ShowInventoryTop();
	/* UI 흔들림 확인 */
	void PlayerHP_Shake(_int _index);
	/* 배고픔 증가 */
	void PlayerHunger_Heal(_float _fHealAmount);
	/* 배고픔 확인 */
	void PlayerHunger_AutoHeal(_float _fTimeDelta);
	/* 체력 회복 */
	void Player_HealHp(_float _fHealAmount);
	/* 배고픔 0일때 체력 - */
	void PlayerHunger_StarvationDamage(_float _fTimeDelta);

	ITEMNAME GetItemTypeName();
	ITEMNAME GetItemTypeName_Left();

public:
	CSlotInfo*						Get_Item(int slotIndex);
	CBag*							Get_Bag()						{ return m_pBag; }
	CItem*							Get_Item()						{ return m_pItem; }
	CCrafting*						Get_Crafting()					{ return m_pCrafting; }
	
	vector<CPlayerHP*>*				Get_vecPlayerHPlist(void)		{ return &m_vecPlayerHPlist; }
	vector<CSlotInfo*>*				Get_vecSlotInfolist(void)		{ return &m_vecSlotInfolist; }
	vector<CSlotInfo*>				Get_vecSlotInfoOriginlist(void)	{ return m_vecSlotInfolist; }
	vector<CCheckBox*>*				Get_vecCheckBoxlist(void)		{ return &m_vecCheckBoxlist; }
	vector<CInventory_Back*>*		Get_vecInventoryBacklist(void)	{ return &m_InventoryBacklist; }
	vector<CSlotInfo*>*				Get_vecSlotFontRenderlist(void) { return &m_vecSlotFontRenderlist; }
	vector<CPlayerLevel*>*			Get_PlayerLevellist(void)		{ return &m_vecPlayerLevellist; }

	list<CPlayerHunger*>*			Get_PlayerHungerlist(void)		{ return &m_PlayerHungerlist; }
	list<CPlayerExp*>*				Get_PlayerExplist(void)			{ return &m_PlayerExplist; }
	list<CMouse_Item*>*				Get_MouseItemlist(void)			{ return &m_MouseItemlist; }
	list<CMouse_ItemFont*>*			Get_MouseItemFontlist(void)		{ return &m_MouseItemFontlist; }

	_bool							Get_PlayerHP_Shader()			{ return m_bPlayerHP_Shader; }

public:
	void Set_Steve(CSteve* _Steve) 
	{ 
		pSteve = _Steve;
		Safe_AddRef(pSteve);
	}

	void			Set_PlayerHP_Shader(_bool _bPlayerHP_Shader)	{ m_bPlayerHP_Shader = _bPlayerHP_Shader; }
	CCrafting*		Set_Crafting(CCrafting* pCrafting)				{ return m_pCrafting = pCrafting; }
	CItem*			Set_Item(CItem* pItem)							{ return m_pItem = pItem; }
	CBag*			Set_Bag(CBag* pBag)								{ return m_pBag = pBag; }
	

private:
	ITEMID	m_ItemID = ITEMID_END;
	_float	m_fHungerTime = { 0.f };
	_int	m_iallZeroCount = { 0 };
	_int	m_iHP_ChangeCount = { 0 };
	_int	m_iLevel = { 1 };
	_bool	m_bPlayerHP_Shader = { false };
	_bool	m_bShake = { false };
	_bool	m_bStarvationStart = { false };

private:
	/* Player State 관련 */
	vector<CPlayerHP*>			m_vecPlayerHPlist;
	vector<CPlayerLevel*>		m_vecPlayerLevellist;
	list<CPlayerHunger*>		m_PlayerHungerlist;
	list<CPlayerExp*>			m_PlayerExplist;
	

	/* Inventory 관련 */
	vector<CSlotInfo*>			m_vecSlotInfolist;
	vector<CSlotInfo*>			m_vecSlotFontRenderlist;
	vector<CCheckBox*>			m_vecCheckBoxlist;
	vector<CInventory_Back*>	m_InventoryBacklist;

	list<CMainInventory*>		m_MainInventorylist;
	list<CSubInventory*>		m_SubInventorylist;
	list<CMouse_Item*>			m_MouseItemlist;
	list<CMouse_ItemFont*>		m_MouseItemFontlist;

	CCrafting*		m_pCrafting = { nullptr };
	CSteve*			pSteve = { nullptr };
	CItem*			m_pItem = { nullptr };
	CBag*			m_pBag = { nullptr };
	
public:
	virtual void Free();
};
END
