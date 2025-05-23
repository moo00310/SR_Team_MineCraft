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
	/* ������, ���� �κ��丮 ����ȭ */
	Synchronize_Slots();
}

void CUI_Mgr::Late_Update(_float fTimeDelta)
{
	ShowInventoryTop();
	PlayerHunger_AutoHeal(fTimeDelta);
	PlayerHunger_Set(fTimeDelta);

	if (m_bStarvationStart)
	{
		PlayerHunger_StarvationDamage(fTimeDelta);
	}
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

			m_vecPlayerHPlist[iIndex]->Set_Flicker(true);
			PlayerHP_Shake(iIndex);
		}
	}
}

void CUI_Mgr::PlayerHunger_Set(_float fTimeDelta)
{
	m_fHungerTime += fTimeDelta;

	if (!m_PlayerHungerlist.empty() && m_fHungerTime >= 15.f)
	{
		m_fHungerTime = 0.f;
		
		for (auto iter = m_PlayerHungerlist.rbegin(); iter != m_PlayerHungerlist.rend(); ++iter)
		{
			/* �� �ֵ��� -> �� �ֵ��� */
			if ((*iter)->Get_TextureNum() == 2)
			{
				(*iter)->Set_TextureNum(0);
			
				(*iter)->Set_Flicker(false);
			
				if(m_iallZeroCount <= 10)
					m_iallZeroCount++;

				break;
			}
			/* Ǯ �ֵ��� -> �� �ֵ��� */
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
	}

	if (m_iallZeroCount == 10 && !m_bStarvationStart)
	{
		m_bStarvationStart = true;
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

		if (texNum == 0) // ��ĭ�϶�
		{
			if (_fHealAmount >= 1.f)
			{
				(*iter)->Set_TextureNum(1); // Ǯ��
				_fHealAmount -= 1.f;

				if (m_iallZeroCount > 0)
					--m_iallZeroCount;
			}
			else if (_fHealAmount >= 0.5f)
			{
				(*iter)->Set_TextureNum(2); // ������
				_fHealAmount -= 0.5f;

				if (m_iallZeroCount > 0)
					--m_iallZeroCount;
			}
		}

		else if (texNum == 2) /* ��ĭ�϶�*/
		{
			if (_fHealAmount >= 0.5f)
			{
				(*iter)->Set_TextureNum(1); // Ǯ��
				_fHealAmount -= 0.5f;
			}
		}
		(*iter)->Set_Flicker(false);
	}
}

void CUI_Mgr::PlayerHunger_AutoHeal(_float _fTimeDelta)
{
	static _float fFullHungerTimer = 0.f;
	static _float fMidHungerTimer = 0.f;

	/* ����� ĭ ���� ��� */
	_int iHungerTotal = 0;
	
	for (auto& hunger : m_PlayerHungerlist)
	{
		/* Ǯ*/
		if (hunger->Get_TextureNum() == 1)
			iHungerTotal += 2;
		/* ���� ����� */
		else if (hunger->Get_TextureNum() == 2)
			iHungerTotal += 1;
	}

	/* ����� ��ġ 20 (��10ĭ 1ĭ �� �������� 2 �����̸� 1)*/
	if (iHungerTotal == 20)
	{
		fFullHungerTimer += _fTimeDelta;

		if (fFullHungerTimer >= 0.5f)
		{
			Player_HealHp(1.0f); 
			fFullHungerTimer = 0.f;
		}
	}
	else if (iHungerTotal >= 18)
	{
		fMidHungerTimer += _fTimeDelta;

		if (fMidHungerTimer >= 4.0f)
		{
			Player_HealHp(0.5f);
			fMidHungerTimer = 0.f;
		}
	}
	else
	{
		fFullHungerTimer = 0.f;
		fMidHungerTimer = 0.f;
	}
}

void CUI_Mgr::Player_HealHp(_float _fHealAmount)
{

	if (_fHealAmount <= 0.f || m_vecPlayerHPlist.empty())
		return;

	for (auto iter = m_vecPlayerHPlist.begin(); iter != m_vecPlayerHPlist.end(); ++iter)
	{
		_int iTextNun = (*iter)->Get_TextureNum();

		/* ��ĭ */
		if (iTextNun == 0)
		{
			if (_fHealAmount >= 1.f)
			{
				(*iter)->Set_TextureNum(1);
				(*iter)->Set_Flicker(true);
				_fHealAmount -= 1.f;
			}
			else if (_fHealAmount >= 0.5f)
			{
				(*iter)->Set_TextureNum(2);
				(*iter)->Set_Flicker(true);
				_fHealAmount -= 0.5f;
			}
		}
		/* ��ĭ -> Ǯĭ*/
		else if (iTextNun == 2 && _fHealAmount >= 0.5f)
		{
			(*iter)->Set_TextureNum(1);
			(*iter)->Set_Flicker(true);
			_fHealAmount -= 0.5f;
		}
	}
}

void CUI_Mgr::PlayerHunger_StarvationDamage(_float _fTimeDelta)
{
	static _float fStarveTimer = 0.f;

	if (m_iallZeroCount == 10)
	{
		fStarveTimer += _fTimeDelta;

		if (fStarveTimer >= 4.0f)
		{
			if (pSteve->Get_Hp() <= 10) return;
			pSteve->Add_Hp(-5);
			SetHP();

			fStarveTimer = 0.f;
		}
	}
	else
	{
		fStarveTimer = 0.f;
		m_bStarvationStart = false;
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

					LevelUp(1);

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

void CUI_Mgr::LevelUp(_int _iLevel)
{
	/* ����ġ �ʱ�ȭ */
	for (auto iter = m_PlayerExplist.begin(); iter != m_PlayerExplist.end(); ++iter)
	{
		(*iter)->Set_RenderOn(false);
	}

	/* ���� ǥ�� ���� ���� */
	m_iLevel += _iLevel;
	
	_int iOnes = m_iLevel % 10;
	_int iTens = m_iLevel / 10;

	m_vecPlayerLevellist[0]->Set_TextureNum(iOnes);

	if (iTens > 0)
	{
		m_vecPlayerLevellist[1]->Set_TextureNum(iTens);
	}
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

	/* �κ��丮 ���Կ� ���� �������� �����ϴ��� Ȯ��
	*  ���� �ε��� 9~47���� */
	/* �κ��丮�� �ش� �������� �����ϴ��� Ȯ��*/
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
	// ���� ������ �� ���Կ� �߰�
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
			// ���� ���� Ȱ��ȭ
			for (int i = 45; i < 56; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(false);
			}

			//ȭ�� ���� ����
			for (int i = 56; i < 59; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}

			//ũ������ ���� ����
			for (int i = 59; i < m_vecSlotInfolist.size(); ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}
		}


		if (g_bFurnaceUiOpen)
		{
			//ȭ�� ���� Ȱ��ȭ
			for (int i = 56; i < 59; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(false);
			}

			// ���� ���� ����
			for (int i = 45; i < 56; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}

			// ũ������ ���� ����
			for (int i = 59; i < m_vecSlotInfolist.size(); ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}
		}


		if (g_bMCraftingTableOpen)
		{
			// ũ������ ���� Ȱ��ȭ
			for (int i = 59; i < m_vecSlotInfolist.size(); ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(false);
			}

			// ���� ���� ����
			for (int i = 45; i < 56; ++i)
			{
				m_vecSlotInfolist[i]->Set_ShowInvenTop(true);
			}

			//ȭ�� ���� ����
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
			m_vecPlayerHPlist[i]->Set_Shake(true);
		}
	}

}

ITEMNAME CUI_Mgr::GetItemTypeName()
{
	// ���� ���� �ε����� ������ �̸��� ������
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
	m_vecPlayerLevellist.clear();
	m_vecCheckBoxlist.clear();
	m_MainInventorylist.clear();
	m_SubInventorylist.clear();
	m_InventoryBacklist.clear();
	m_MouseItemlist.clear();
	m_MouseItemFontlist.clear();
	m_vecSlotFontRenderlist.clear();
}
