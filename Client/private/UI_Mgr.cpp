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
	PlayerHunger_Set(fTimeDelta);
}

void CUI_Mgr::Synchronize_Slots()
{
	if (m_vecSlotInfolist.size() != 0)
	{
		/* ITEMNAME_WOOD, ITEMNAME_OAKPLANKS, ITEMNAME_COBBLESTONE, ITEMNAME_DIRT, ITEMNAME_DANDELION, ITEMNAME_END */
		
		for (int i = 0; i < 9; ++i)
		{
			if (m_vecSlotInfolist[i] && m_vecSlotInfolist[i + 9])
			{
				m_vecSlotInfolist[i]->Set_ItemName(m_vecSlotInfolist[i + 9]->Get_ItemName());
				m_vecSlotInfolist[i]->Set_ItemCount(m_vecSlotInfolist[i + 9]->Get_ItemCount());
			}
		}
	}
}

void CUI_Mgr::SetHP()
{
	_int SteveHp = pSteve->Get_Hp();
	
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
			/* �� �ֵ��� -> �� �ֵ��� */
			if ((*iter)->Get_TextureNum() == 2)
			{
				(*iter)->Set_TextureNum(0);
			
				(*iter)->Set_Flicker(false);
			
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

		if (m_iallZeroCount == 10)
		{
			SetHP();
			/* ���� �Ծ ����� ȸ��?�ϸ� m_iallZeroCount �ʱ�ȭ */
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
			/* ������ back -> 5���ȴٸ� LevelUp() ȣ�� */
		}
	}
}

void CUI_Mgr::LevelUp()
{
	/* ���� ǥ�� ���� ���� */
	for (auto iter = m_PlayerExplist.begin(); iter != m_PlayerExplist.end(); ++iter)
	{
		(*iter)->Set_RenderOn(false);
	}
}

void CUI_Mgr::ItemCount_Update(ITEMNAME _ItemName, _int AddCount)
{
	/* �κ��丮 ���Կ� ���� �������� �����ϴ��� Ȯ�� 
	*  ���� �ε��� 9~47���� */

	/* �κ��丮�� �ش� �������� �����ϴ��� Ȯ��*/
	for (int i = 9; i < m_vecSlotInfolist.size(); ++i)
	{
		if (m_vecSlotInfolist[i]->Get_ItemName() == _ItemName)
		{
			if (AddCount + m_vecSlotInfolist[i]->Get_ItemCount() < 65)
			{
				m_vecSlotInfolist[i]->Set_ItemCount(m_vecSlotInfolist[i]->Get_ItemCount() + AddCount);
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

	/* ���Կ� �������� ������ �߰� */
	for (auto iter : vecSlotOrder)
	{
		/* ���Կ� �������� ���������*/
		if (m_vecSlotInfolist[iter]->Get_ItemName() == ITEMNAME_END)
		{
			m_vecSlotInfolist[iter]->Set_ItemName(_ItemName);
			m_vecSlotInfolist[iter]->Set_ItemCount(AddCount);
			break;
		}
	}
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

