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
	Synchronize_Slots();
}

void CUI_Mgr::Late_Update(_float fTimeDelta)
{
	PlayerHunger_Set(fTimeDelta);
}

void CUI_Mgr::Synchronize_Slots()
{
	if (m_vecItemlist.size() != 0)
	{
		/* ITEMNAME_WOOD, ITEMNAME_OAKPLANKS, ITEMNAME_COBBLESTONE, ITEMNAME_DIRT, ITEMNAME_DANDELION, ITEMNAME_END */
		CItem* pItem9 = m_vecItemlist[9];
		CItem* pItem10 = m_vecItemlist[10];
		CItem* pItem11 = m_vecItemlist[11];
		CItem* pItem15 = m_vecItemlist[15];
		CItem* pItem17 = m_vecItemlist[17];
		/*=====================================*/
		/* 테스트용 */
		if (!pItem9->Get_Test())
		{
			pItem9->Set_ItemName(ITEMNAME_COBBLESTONE);
		}
		if (!pItem10->Get_Test())
		{
			pItem10->Set_ItemName(ITEMNAME_WOOD);
		}
		if (!pItem11->Get_Test())
		{
			pItem11->Set_ItemName(ITEMNAME_OAKPLANKS);
		}
		if (!pItem15->Get_Test())
		{
			pItem15->Set_ItemName(ITEMNAME_DIRT);
		}
		if (!pItem17->Get_Test())
		{
			pItem17->Set_ItemName(ITEMNAME_DANDELION);
		}
		/*=====================================*/

		for (int i = 0; i < 9; ++i)
		{
			if (m_vecItemlist[i] && m_vecItemlist[i + 9])
			{
				m_vecItemlist[i]->Set_ItemName(m_vecItemlist[i + 9]->Get_ItemName());
			}
		}
	}
}

void CUI_Mgr::TakeDamge()
{
	if (!m_PlayerHPlist.empty())
	{
		for (auto iter = m_PlayerHPlist.rbegin(); iter != m_PlayerHPlist.rend(); ++iter)
		{

			if ((*iter)->Get_TextureNum() == 2)
			{
				(*iter)->Set_TextureNum(0);
				break;
			}
			if ((*iter)->Get_TextureNum() == 1)
			{
				(*iter)->Set_TextureNum(2);
				break;
			}
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
			TakeDamge();
			
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

void CUI_Mgr::Add_Item(CItem* pItem)
{
	m_vecItemlist.push_back(pItem);
}

CItem* CUI_Mgr::Get_Item(int slotIndex)
{
	if (slotIndex >= 0 && slotIndex < m_vecItemlist.size())
	{
		return m_vecItemlist[slotIndex];
	}

	return nullptr;
}

void CUI_Mgr::Free()
{
	m_vecItemlist.clear();
	m_PlayerHPlist.clear();
	m_PlayerHungerlist.clear();
	m_PlayerExplist.clear();
}

