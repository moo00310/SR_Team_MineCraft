#include "UI_Mgr.h"
#include "GameInstance.h"
#include "PlayerHP.h"

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

}

void CUI_Mgr::Synchronize_Slots()
{
	if (m_vecItemlist.size() != 0)
	{
		CItem* pItem9 = m_vecItemlist[9];
		CItem* pItem10 = m_vecItemlist[10];
		/*=====================================*/
		/* 테스트용 */
		pItem9->Set_ItemType(ITEMNAME_WOOD);
		pItem10->Set_ItemType(ITEMNAME_AXE);
		/*=====================================*/
		for (int i = 0; i < 9; ++i)
		{
			if (m_vecItemlist[i] && m_vecItemlist[i + 9])
			{
				m_vecItemlist[i]->Set_ItemType(m_vecItemlist[i + 9]->Get_ItemType());
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
			if ((*iter)->Get_TextureNum() == 1)
			{
				(*iter)->Set_TextureNum(2);
				break;
			}
		}
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
}

