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
			}

			m_bPlayerHP_Shader = true;
		}
	}
}

void CUI_Mgr::PlayerHunger_Set(_float fTimeDelta)
{
	m_fHungerTime += fTimeDelta;

	if (!m_PlayerHungerlist.empty() && m_fHungerTime >= 10.f)
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

		if (m_iallZeroCount == 10)
		{
			SetHP();
			/* 음식 먹어서 배고픔 회복?하면 m_iallZeroCount 초기화 */
		}
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

					//LevelUp();

					break;
				}
				else
				{
					(*iter)->Set_TextureNum(4);
					(*iter)->Set_RenderOn(true);
					break;
				}
			}
			/* 마지막 back -> 5가된다면 LevelUp() 호출 */
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
}

void CUI_Mgr::ItemCount_Update(ITEMNAME _ItemName, _int AddCount)
{
	/*
	enum ITEMNAME
	{
		ITEMNAME_DIRT, ITEMNAME_GRASSDIRT, ITEMNAME_STONE, ITEMNAME_COBBLESTONE, ITEMNAME_WOOD, ITEMNAME_OAKPLANKS, ITEMNAME_LEAF, 
		ITEMNAME_CUBE_END,
		
		ITEMNAME_SEED = 100, ITEMNAME_SAPLING, ITEMNAME_APPLE, ITEMNAME_REDTULIP, ITEMNAME_DANDELION, ITEMNAME_COAL, ITEMNAME_RAWIRON, ITEMNAME_IRON,
		ITEMNAME_SWORD, ITEMNAME_PICKAXE, ITEMNAME_TORCH, ITEM_WEPON_1, ITEM_WEPON_2, ITEM_WEPON_3, ITEM_WEPON_4, ITEM_WEPON_5, ITEMNAME_RECT_END,


		ITEMNAME_IRONORE = 900, ITEMNAME_COALORE, ITEMNAME_GRASS,
		ITEMNAME_END = 999
	};

	*/

	if (_ItemName < ITEMNAME_CUBE_END)
	{
		m_ItemID = ITEMID_BLOCK;
	}
	
	else if (_ItemName >= 108 && _ItemName < 115)
	{
		m_ItemID = ITEMID_ARMOR;
	}

	/* 인벤토리 슬롯에 같은 아이템이 존재하는지 확인
	*  슬롯 인덱스 9~47까지 */
	/* 인벤토리에 해당 아이템이 존재하는지 확인*/
	for (int i = 9; i < m_vecSlotInfolist.size(); ++i)
	{
		if (m_vecSlotInfolist[i]->Get_ItemName() == _ItemName)
		{
			if (AddCount + m_vecSlotInfolist[i]->Get_ItemCount() < 65 && m_ItemID != ITEMID_ARMOR)
			{
				m_vecSlotInfolist[i]->Set_ItemCount(m_vecSlotInfolist[i]->Get_ItemCount() + AddCount);
				//m_vecSlotInfolist[i]->Set_ItemID(m_ItemID);
				return;
			}
			else
			{
				break;
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

	/* 슬롯에 아이템이 없을때 추가 */
	for (auto iter : vecSlotOrder)
	{
		/* 슬롯에 아이템이 비어있을때*/
		if (m_vecSlotInfolist[iter]->Get_ItemName() == ITEMNAME_END)
		{
			m_vecSlotInfolist[iter]->Set_ItemName(_ItemName);
			m_vecSlotInfolist[iter]->Set_ItemCount(AddCount);
			m_vecSlotInfolist[iter]->Set_ItemID(m_ItemID);
			m_vecSlotInfolist[iter]->Set_ItemCountRender(true);
			break;
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
}

