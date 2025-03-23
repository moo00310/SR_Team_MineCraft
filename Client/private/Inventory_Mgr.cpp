#include "Inventory_Mgr.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CInventory_Mgr)

CInventory_Mgr::CInventory_Mgr()
{
}

void CInventory_Mgr::Priority_Update(_float fTimeDelta)
{
}

void CInventory_Mgr::Update(_float fTimeDelta)
{
	Synchronize_Slots();
}

void CInventory_Mgr::Late_Update(_float fTimeDelta)
{

}

void CInventory_Mgr::Synchronize_Slots()
{
	/*for (int i = 0; i < 9; ++i)
	{
		if (m_vecItemlist[i] && m_vecItemlist[i + 9])
		{
			m_vecItemlist[i]->Set_ItemType(m_vecItemlist[i + 9]->Get_ItemType());
		}
	}*/

	if (m_vecItemlist.size() > 9)
	{
		CItem* pItem0 = m_vecItemlist[0];
		CItem* pItem9 = m_vecItemlist[9];

		if (pItem0 && pItem9)
		{
			pItem0->Get_ItemType();
			pItem9->Get_ItemType();

			pItem0->Set_ItemType(pItem9->Get_ItemType());

			pItem0->Get_ItemType();
		}
	}
}

void CInventory_Mgr::Add_Item(CItem* pItem)
{
	m_vecItemlist.push_back(pItem);
}

CItem* CInventory_Mgr::Get_Item(int slotIndex)
{
	if (slotIndex >= 0 && slotIndex < m_vecItemlist.size())
	{
		return m_vecItemlist[slotIndex];
	}

	return nullptr;
}
