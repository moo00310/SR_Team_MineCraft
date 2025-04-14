#include "UI_Mgr.h"
#include "GameInstance.h"


IMPLEMENT_SINGLETON(CUI_Mgr)

CUI_Mgr::CUI_Mgr()
{
}

void CUI_Mgr::Priority_Update(_float fTimeDelta)
{
}

void CUI_Mgr::Update(_float fTimeDelta)
{
	/* 퀵슬롯, 메인 인벤토리 동기화 */
	Synchronize_Slots();
}

void CUI_Mgr::Late_Update(_float fTimeDelta)
{
	ShowInventoryTop();
	PlayerHunger_Set(fTimeDelta);
}

void CUI_Mgr::Synchronize_Slots()
{
	if (m_vecSlotInfolist.size() != 0)
	{	
		for (int i = 0; i < 9; ++i)
		{
			if (m_vecSlotInfolist[i] && m_vecSlotInfolist[i + 9])
			{
				m_vecSlotInfolist[i]->Set_ItemName(m_vecSlotInfolist[i + 9]->Get_ItemName());
				m_vecSlotInfolist[i]->Set_ItemCount(m_vecSlotInfolist[i + 9]->Get_ItemCount());
				m_vecSlotInfolist[i]->Set_ItemCountRender(m_vecSlotInfolist[i + 9]->Get_ItemCountRender());
			}
		}
	}
}

void CUI_Mgr::SetHP()
{
	_int SteveHp = static_cast<_int>(pSteve->Get_Hp());
	
	if (!m_vecPlayerHPlist.empty())
	{
		if (SteveHp <= 0)
		{
			for (auto& Hp : m_vecPlayerHPlist)
				Hp->Set_TextureNum(0);

			return;
		}

		if (!m_vecPlayerHPlist.empty())
		{
			_int iTensPlace = SteveHp / 10;
			_int iOnesPlace = SteveHp % 10;
			_int iCount = { 0 };
			_int iIndex = { 0 };

			for (int i = iTensPlace; i < m_vecPlayerHPlist.size(); ++i)
			{
				m_vecPlayerHPlist[i]->Set_TextureNum(0);
				
				if (iCount == 0)
				{
					iIndex = i;
				}

				iCount++;
			}

			if (iOnesPlace != 0 && SteveHp > 0) 
			{
				m_vecPlayerHPlist[iIndex]->Set_TextureNum(2);
				m_vecPlayerHPlist[iIndex]->Set_Flicker(true);
			}

			PlayerHP_Shake(iIndex);
		}
	}
}

void CUI_Mgr::PlayerHunger_Set(_float fTimeDelta)
{
	m_fHungerTime += fTimeDelta;

	if (!m_PlayerHungerlist.empty() && m_fHungerTime >= 5.f)
	{
		m_fHungerTime = 0.f;
		
		for (auto iter = m_PlayerHungerlist.rbegin(); iter != m_PlayerHungerlist.rend(); ++iter)
		{
			/* 반 핫도그 -> 빈 핫도그 */
			if ((*iter)->Get_TextureNum() == 2)
			{
				(*iter)->Set_TextureNum(0);
			
				(*iter)->Set_Flicker(false);
			
				m_iallZeroCount++;

				break;
			}
			/* 풀 핫도그 -> 반 핫도그 */
			if ((*iter)->Get_TextureNum() == 1)
			{
				(*iter)->Set_TextureNum(2);

				if (!(*iter)->Get_Flicker())
				{
					(*iter)->Set_Flicker(true);
				}

				break;
			}
		}

		/*if (m_iallZeroCount == 10)
		{
			SetHP();
		}*/
	}
}

void CUI_Mgr::PlayerHunger_Heal(_float _fHealAmount)
{
	if (m_PlayerHungerlist.empty() || _fHealAmount <= 0.f)
		return;

	for (auto iter = m_PlayerHungerlist.begin(); iter != m_PlayerHungerlist.end(); ++iter)
	{
		if (_fHealAmount <= 0.f)
			break;

		_int texNum = (*iter)->Get_TextureNum();

		if (texNum == 0) // 빈칸일때
		{
			if (_fHealAmount >= 1.f)
			{
				(*iter)->Set_TextureNum(1); // 풀로
				_fHealAmount -= 1.f;
			}
			else if (_fHealAmount >= 0.5f)
			{
				(*iter)->Set_TextureNum(2); // 반으로
				_fHealAmount -= 0.5f;
			}
		}

		else if (texNum == 2) /* 반칸일때*/
		{
			if (_fHealAmount >= 0.5f)
			{
				(*iter)->Set_TextureNum(1); // 반으로
				_fHealAmount -= 0.5f;
			}
		}
		(*iter)->Set_Flicker(false);
	}
}

void CUI_Mgr::PlayerExp_Set()
{
	if (!m_PlayerExplist.empty())
	{
		for (auto iter = m_PlayerExplist.begin(); iter != m_PlayerExplist.end(); ++iter)
		{
			if (!(*iter)->Get_RenderOn())
			{
				if ((*iter)->Get_ExpIndex() == 0)
				{
					(*iter)->Set_TextureNum(3);
					(*iter)->Set_RenderOn(true);
					break;
				}
				else if ((*iter)->Get_ExpIndex() == 17)
				{
					(*iter)->Set_TextureNum(5);
					(*iter)->Set_RenderOn(true);

					LevelUp();

					break;
				}
				else
				{
					(*iter)->Set_TextureNum(4);
					(*iter)->Set_RenderOn(true);
					break;
				}
			}
		}
	}
}

void CUI_Mgr::LevelUp()
{
	/* 레벨 표기 숫자 변경 */
	for (auto iter = m_PlayerExplist.begin(); iter != m_PlayerExplist.end(); ++iter)
	{
		(*iter)->Set_RenderOn(false);
	}

	/*for (auto iter = m_PlayerLevellist.begin(); iter != m_PlayerLevellist.end() ++iter)
	{

	}*/
}

void CUI_Mgr::ItemCount_Update(ITEMNAME _ItemName, _int AddCount)
{
	
	/*	ITEMNAME_DIRT, ITEMNAME_GRASSDIRT, ITEMNAME_STONE, ITEMNAME_COBBLESTONE, ITEMNAME_WOOD, ITEMNAME_OAKPLANKS, ITEMNAME_LEAF,
		ITEMNAME_CUBE_END,
		
		ITEMNAME_SEED = 100, ITEMNAME_SAPLING, ITEMNAME_APPLE, ITEMNAME_REDTULIP, ITEMNAME_DANDELION, ITEMNAME_COAL, ITEMNAME_RAWIRON, ITEMNAME_IRON,
		ITEMNAME_TORCH, ITEMNAME_PICKAXE, ITEMNAME_SWORD, ITEM_WEPON_1, ITEM_WEPON_2, ITEM_WEPON_3, ITEM_WEPON_4, ITEM_WEPON_5, ITEMNAME_RECT_END,

		ITEMNAME_IRONORE = 900, ITEMNAME_COALORE, ITEMNAME_GRASS,
		ITEMNAME_END = 999
	*/

	if (_ItemName < ITEMNAME_CUBE_END)
	{
		m_ItemID = ITEMID_BLOCK;
	}
	else if (_ItemName >= ITEMNAME_WOOD_PICKAXE && _ItemName < ITEMNAME_RECT_END)
		m_ItemID = ITEMID_ARMOR;
	else
		m_ItemID = ITEMID_END;


	if (AddCount == -1)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (m_vecSlotInfolist[i]->Get_ItemName() == _ItemName)
			{
				_int iCurrentCount = m_vecSlotInfolist[i]->Get_ItemCount();

				if (iCurrentCount > 1)
				{	
					m_vecSlotInfolist[i]->Set_ItemCount(iCurrentCount + AddCount);			
					m_vecSlotInfolist[i + 9]->Set_ItemCount(m_vecSlotInfolist[i]->Get_ItemCount());
				}
				else
				{
					m_vecSlotInfolist[i]->Set_ItemCount(0);
					m_vecSlotInfolist[i]->Set_ItemName(ITEMNAME_END);
					m_vecSlotInfolist[i]->Set_ItemID(ITEMID_END);
					m_vecSlotInfolist[i]->Set_ItemCountRender(false);

					m_vecSlotInfolist[i + 9]->Set_ItemCount(m_vecSlotInfolist[i]->Get_ItemCount());
					m_vecSlotInfolist[i + 9]->Set_ItemName(m_vecSlotInfolist[i]->Get_ItemName());
					m_vecSlotInfolist[i + 9]->Set_ItemID(m_vecSlotInfolist[i]->Get_ItemID());
					m_vecSlotInfolist[i + 9]->Set_ItemCountRender(m_vecSlotInfolist[i]->Get_ItemCountRender());

				}
				return;
			}
		}
		return;
	}

	_int iRemain = AddCount;

	/* 인벤토리 슬롯에 같은 아이템이 존재하는지 확인
	*  슬롯 인덱스 9~47까지 */
	/* 인벤토리에 해당 아이템이 존재하는지 확인*/
	for (int i = 9; i < m_vecSlotInfolist.size(); ++i)
	{
		if (m_vecSlotInfolist[i]->Get_ItemName() == _ItemName && m_ItemID != ITEMID_ARMOR)
		{
			_int currentCount = m_vecSlotInfolist[i]->Get_ItemCount();
			if (currentCount < 64)
			{
				_int space = 64 - currentCount;

				if (iRemain <= space)
				{
					m_vecSlotInfolist[i]->Set_ItemCount(currentCount + iRemain);
					return;
				}
				else
				{
					m_vecSlotInfolist[i]->Set_ItemCount(64);
					iRemain -= space;
				}
			}
		}
	}

	static const vector<int> vecSlotOrder =
	{
		 9, 10, 11, 12, 13, 14, 15, 16, 17,
		36, 37, 38, 39, 40, 41, 42, 43, 44,
		27, 28, 29, 30, 31, 32, 33, 34, 35,
		18, 19, 20, 21, 22, 23, 24, 25, 26

	};
	// 남은 수량을 빈 슬롯에 추가
	for (auto iter : vecSlotOrder)
	{
		if (iRemain <= 0)
			break;

		if (m_vecSlotInfolist[iter]->Get_ItemName() == ITEMNAME_END)
		{
			if (iRemain <= 64)
			{
				m_vecSlotInfolist[iter]->Set_ItemName(_ItemName);
				m_vecSlotInfolist[iter]->Set_ItemCount(iRemain);
				m_vecSlotInfolist[iter]->Set_ItemID(m_ItemID);
				m_vecSlotInfolist[iter]->Set_ItemCountRender(true);
				break;
			}
			else
			{
				m_vecSlotInfolist[iter]->Set_ItemName(_ItemName);
				m_vecSlotInfolist[iter]->Set_ItemCount(64);
				m_vecSlotInfolist[iter]->Set_ItemID(m_ItemID);
				m_vecSlotInfolist[iter]->Set_ItemCountRender(true);
				iRemain -= 64;
			}
		}
	}
}

void CUI_Mgr::ShowInventoryTop()
{
	if (!m_vecSlotInfolist.empty())
	{
		if (g_bMainInventoryOpen) {
			// 메인 슬롯 활성화
			for (int i = 45; i < 56; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(false);
			}

			//화로 슬롯 무시
			for (int i = 56; i < 59; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}

			//크래프팅 슬롯 무시
			for (int i = 59; i < m_vecSlotInfolist.size(); ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}
		}


		if (g_bFurnaceUiOpen)
		{
			//화로 슬롯 활성화
			for (int i = 56; i < 59; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(false);
			}

			// 메인 슬롯 무시
			for (int i = 45; i < 56; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}

			// 크래프팅 슬롯 무시
			for (int i = 59; i < m_vecSlotInfolist.size(); ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}
		}


		if (g_bMCraftingTableOpen)
		{
			// 크래프팅 슬롯 활성화
			for (int i = 59; i < m_vecSlotInfolist.size(); ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(false);
			}

			// 메인 슬롯 무시
			for (int i = 45; i < 56; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}

			//화로 슬롯 무시
			for (int i = 56; i < 59; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}
		}
	}
}

void CUI_Mgr::PlayerHP_Shake(_int _index)
{
	if (!m_vecPlayerHPlist.empty())
	{
		for (int i = _index; i < m_vecPlayerHPlist.size(); ++i)
		{
			m_vecPlayerHPlist[i]->Set_Flicker(true);
		}
	}

}


ITEMNAME CUI_Mgr::GetItemTypeName()
{
	// 현재 슬록 인덱스의 아이템 이름을 가져옴
	return m_vecSlotInfolist[(*Get_vecCheckBoxlist()->begin())->Get_CheckBoxIndex()]->Get_ItemName();
}

ITEMNAME CUI_Mgr::GetItemTypeName_Left()
{
	return m_vecSlotInfolist[50]->Get_ItemName();
}
 
void CUI_Mgr::Add_SlotUpdate(CSlotInfo* pItem)
{
	m_vecSlotInfolist.push_back(pItem);
}

CSlotInfo* CUI_Mgr::Get_Item(int slotIndex)
{
	if (slotIndex >= 0 && slotIndex < m_vecSlotInfolist.size())
	{
		return m_vecSlotInfolist[slotIndex];
	}

	return nullptr;
}

void CUI_Mgr::Free()
{
	__super::Free();
	Safe_Release(pSteve);
	m_vecSlotInfolist.clear();
	m_vecPlayerHPlist.clear();
	m_PlayerHungerlist.clear();
	m_PlayerExplist.clear();
	m_PlayerLevellist.clear();
	m_vecCheckBoxlist.clear();
	m_MainInventorylist.clear();
	m_SubInventorylist.clear();
	m_InventoryBacklist.clear();
	m_MouseItemlist.clear();
	m_MouseItemFontlist.clear();
	m_vecSlotFontRenderlist.clear();
}
