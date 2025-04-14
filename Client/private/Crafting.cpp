#include "Crafting.h"
#include "UI_Mgr.h"


CCrafting::CCrafting(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CCrafting::CCrafting(CCrafting& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CCrafting::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCrafting::Initialize(void* pArg)
{
	/* 아이템 레시피 조합법 정의 */
	/*
		_int itemCountPerSlot = { 0 }; // 슬롯당 필요한 아이템 개수
		_int iTotalInputCout = { 0 }; // 조합에 들어가는 총 재료 개수
		_int resultCount = { 0 }; // 레시피 조합 후 결과 아이템 개수
	*/

	RECIPE_DESC oakplanks1 = { {{51, ITEMNAME_WOOD}}, ITEMNAME_OAKPLANKS, 1, 1, 4 };
	RECIPE_DESC oakplanks2 = { {{52, ITEMNAME_WOOD}}, ITEMNAME_OAKPLANKS, 1, 1, 4 };
	RECIPE_DESC oakplanks3 = { {{53, ITEMNAME_WOOD}}, ITEMNAME_OAKPLANKS, 1, 1, 4 };
	RECIPE_DESC oakplanks4 = { {{54, ITEMNAME_WOOD}}, ITEMNAME_OAKPLANKS, 1, 1, 4 };
	RECIPE_DESC stick1 = { {{51, ITEMNAME_OAKPLANKS}, {53, ITEMNAME_OAKPLANKS}}, ITEMNAME_STICK, 1, 2, 4};
	RECIPE_DESC stick2 = { {{52, ITEMNAME_OAKPLANKS}, {54, ITEMNAME_OAKPLANKS}}, ITEMNAME_STICK, 1, 2, 4 };
	
	RECIPE_DESC  Torch1 = { {{51, ITEMNAME_COAL}, {53, ITEMNAME_STICK}}, ITEMNAME_TORCH, 1, 2, 4 };
	RECIPE_DESC  Torch2 = { {{52, ITEMNAME_COAL}, {54, ITEMNAME_STICK}}, ITEMNAME_TORCH, 1, 2, 4 };

	RECIPE_DESC CraftingTable = { {{51, ITEMNAME_OAKPLANKS}, {52, ITEMNAME_OAKPLANKS}, {53, ITEMNAME_OAKPLANKS}, {54, ITEMNAME_OAKPLANKS}},
	ITEMNAME_CRAFTINGTABLE, 1, 4, 1};
	
	RECIPE_DESC WoodPickAxe = { {{59, ITEMNAME_OAKPLANKS}, {60, ITEMNAME_OAKPLANKS}, {61, ITEMNAME_OAKPLANKS}, {63, ITEMNAME_STICK}, {66, ITEMNAME_STICK}},
	ITEMNAME_WOOD_PICKAXE, 1, 5, 1};

	RECIPE_DESC StonePickAxe = { {{59, ITEMNAME_COBBLESTONE}, {60, ITEMNAME_COBBLESTONE}, {61, ITEMNAME_COBBLESTONE}, {63, ITEMNAME_STICK}, {66, ITEMNAME_STICK}},
	ITEMNAME_STONE_PICKAXE, 1, 5, 1 };
	
	RECIPE_DESC StoneAxe = { {{59, ITEMNAME_COBBLESTONE}, {60, ITEMNAME_COBBLESTONE}, {62, ITEMNAME_COBBLESTONE}, {63, ITEMNAME_STICK}, {66, ITEMNAME_STICK}},
	ITEMNAME_STONE_AXE, 1, 5, 1 };
	
	RECIPE_DESC StonnSword = { {{60, ITEMNAME_COBBLESTONE}, {63, ITEMNAME_COBBLESTONE}, {66, ITEMNAME_STICK}},
	ITEMNAME_STONE_SWORD, 1, 3, 1 };

	RECIPE_DESC SteelSword = { {{60, ITEMNAME_IRON}, {63, ITEMNAME_IRON}, {66, ITEMNAME_STICK}},
	ITEMNAME_STEEL_SWORD, 1, 3, 1 };

	RECIPE_DESC Furance = { {{59, ITEMNAME_COBBLESTONE}, {60, ITEMNAME_COBBLESTONE}, {61, ITEMNAME_COBBLESTONE}, {62, ITEMNAME_COBBLESTONE}, {64, ITEMNAME_COBBLESTONE}, {65, ITEMNAME_COBBLESTONE},
		{66, ITEMNAME_COBBLESTONE}, {67, ITEMNAME_COBBLESTONE}},ITEMNAME_FURANCE, 1, 8, 1 };

	RECIPE_DESC Wepon1 = { {{60, ITEMNAME_ROTTENFLESH}, {63, ITEMNAME_GUNPOWDER}, {66, ITEMNAME_STEEL_SWORD}},ITEM_WEPON_1, 1, 3, 1 };


	m_vecRecipelist.push_back(oakplanks1);
	m_vecRecipelist.push_back(oakplanks2);
	m_vecRecipelist.push_back(oakplanks3);
	m_vecRecipelist.push_back(oakplanks4);
	m_vecRecipelist.push_back(stick2);
	m_vecRecipelist.push_back(stick1);
	m_vecRecipelist.push_back(Torch1);
	m_vecRecipelist.push_back(Torch2);
	m_vecRecipelist.push_back(CraftingTable);
	m_vecRecipelist.push_back(WoodPickAxe);
	m_vecRecipelist.push_back(StonePickAxe);
	m_vecRecipelist.push_back(StoneAxe);
	m_vecRecipelist.push_back(StonnSword);
	m_vecRecipelist.push_back(SteelSword);
	m_vecRecipelist.push_back(Furance);
	m_vecRecipelist.push_back(Wepon1);

	return S_OK;
}

void CCrafting::Priority_Update(_float fTimeDelta)
{
}

void CCrafting::Update(_float fTimeDelta)
{
}

void CCrafting::Late_Update(_float fTimeDelta)
{
}

HRESULT CCrafting::Render()
{
	return S_OK;
}

void CCrafting::Crafing()
{
	CUI_Mgr* pUI_Mgr = CUI_Mgr::Get_Instance();

	if (pUI_Mgr->Get_vecSlotInfolist()->at(55)->Get_ItemCount() > 0)
		return;

	if (pUI_Mgr->Get_vecSlotInfolist()->at(68)->Get_ItemCount() > 0)
		return;


	for (const auto& recipe : m_vecRecipelist)
	{
		m_bMatch = true;

		for (const auto& pair : recipe.inputPattern)
		{
			/* 레시피 (슬롯 인덱스와 슬롯의 재료)*/
			_int iSlotIndex = pair.first;
			ITEMNAME  requiredItem = pair.second;

			if (pUI_Mgr->Get_vecSlotInfolist()->at(iSlotIndex)->Get_ItemName() != requiredItem)
			{
				m_bMatch = false;
				break;
			}
		}

		if (m_bMatch)
		{
			if (g_bMainInventoryOpen)
			{
				for (int i = 51; i < 55; ++i)
				{
					if (recipe.inputPattern.count(i) > 0)
						continue;

					if (pUI_Mgr->Get_vecSlotInfolist()->at(i)->Get_ItemName() != ITEMNAME_END)
					{
						m_bMatch = false;
						break;
					}
				}
			}
			
			if (g_bMCraftingTableOpen)
			{
				for (int i = 59; i < 68; ++i)
				{
					if (recipe.inputPattern.count(i) > 0)
						continue;

					if (pUI_Mgr->Get_vecSlotInfolist()->at(i)->Get_ItemName() != ITEMNAME_END)
					{
						m_bMatch = false;
						break;
					}
				}
			}
	
		}
		if (m_bMatch)
		{
			/* 매칭된 레시피 저장 */
			m_pMatchedRecipe = &recipe;
			/* 필요한 재료 총 개수 */
			m_iItemTotalCount = recipe.iTotalInputCout;
			/* 필요한 슬롯의 개수 합 0*/
			m_iSlotTotalCount = 0; 
			/* 완성 개수 */
			m_iresultCount = recipe.resultCount; 
			/* 슬롯당 필요 개수*/
			m_itemCountPerSlot = recipe.itemCountPerSlot;
		
			for (const auto& pair : recipe.inputPattern)
			{
				m_iSlotIndex = pair.first;

				/* 슬롯에 들어간 아이템 개수 확인 */
				//m_iTempCount = pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndex)->Get_ItemCount();
				m_iSlotTotalCount += m_iTempCount;

				/* 필요한 개수 마이너스 */
			/*	m_iTempCount -= recipe.itemCountPerSlot;

				if (m_iTempCount <= 0)
				{
					pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndex)->Set_ItemName(ITEMNAME_END);
				}
				else
				{
					pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndex)->Set_ItemCount(m_iTempCount);
				}*/
			}

			if (g_bMCraftingTableOpen)
			{
				pUI_Mgr->Get_vecSlotInfolist()->at(68)->Set_ItemName(recipe.resultItem);
				pUI_Mgr->Get_vecSlotInfolist()->at(68)->Set_ItemCount(recipe.resultCount);
				pUI_Mgr->Get_vecSlotInfolist()->at(68)->Set_ItemCountRender(true);
			}
			else if (g_bMainInventoryOpen)
			{
				/* 레시피 조합 결과 아이템을 55번 슬롯에 넣기 */
				pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemName(recipe.resultItem);
				pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCount(recipe.resultCount);
				pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCountRender(true);
				//pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemID(ITEMID_END);
			}
			
			break;
		}
	}
}

void CCrafting::RButton()
{
	CUI_Mgr* pUI_Mgr = CUI_Mgr::Get_Instance();

	if (pUI_Mgr->Get_vecSlotInfolist()->at(55)->Get_ItemCount() <= 0)
		return;

	if (m_bMatch && m_pMatchedRecipe != nullptr)
	{
		/*
			_int itemCountPerSlot = { 0 }; // 슬롯당 필요한 아이템 개수
			_int iTotalInputCout = { 0 }; // 조합에 들어가는 총 재료 개수
			_int resultCount = { 0 }; // 레시피 조합 후 결과 아이템 개수
		*/
		_int iItemCount = { 0 };
		_bool _bRun = { false };
		
		while (true)
		{
			for (const auto& pair : m_pMatchedRecipe->inputPattern)
			{
				_int SlotIndex = pair.first;

				if (pUI_Mgr->Get_vecSlotInfolist()->at(SlotIndex)->Get_ItemCount() == 0)
				{
					_bRun = true;
				}

			}
		
			if (_bRun)
			{

				pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCount(iItemCount * m_iresultCount);
				break;
			}
			
			iItemCount++;

			for (const auto& pair : m_pMatchedRecipe->inputPattern)
			{	
				// 51, 52, 53, 54
				_int SlotIndex = pair.first;
				_int GetItemCount = pUI_Mgr->Get_vecSlotInfolist()->at(SlotIndex)->Get_ItemCount();

				GetItemCount -= m_itemCountPerSlot;
				pUI_Mgr->Get_vecSlotInfolist()->at(SlotIndex)->Set_ItemCount(GetItemCount);

				if (GetItemCount == 0)
					pUI_Mgr->Get_vecSlotInfolist()->at(SlotIndex)->Set_ItemName(ITEMNAME_END);
 			}
		}
	}
}

void CCrafting::ConsumeRecipeItems()
{
	if (!m_pMatchedRecipe)
		return;

	CUI_Mgr* pUI_Mgr = CUI_Mgr::Get_Instance();

	for (const auto& pair : m_pMatchedRecipe->inputPattern)
	{
		_int iSlotIndex = pair.first;
		_int currentCount = pUI_Mgr->Get_vecSlotInfolist()->at(iSlotIndex)->Get_ItemCount();

		currentCount -= m_itemCountPerSlot;

		if (currentCount <= 0)
		{
			pUI_Mgr->Get_vecSlotInfolist()->at(iSlotIndex)->Set_ItemName(ITEMNAME_END);
			pUI_Mgr->Get_vecSlotInfolist()->at(iSlotIndex)->Set_ItemCount(0);
		}
		else
		{
			pUI_Mgr->Get_vecSlotInfolist()->at(iSlotIndex)->Set_ItemCount(currentCount);
		}
	}
}


HRESULT CCrafting::Ready_Components()
{
	return S_OK;
}

CCrafting* CCrafting::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCrafting* pInstance = new CCrafting(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCrafting");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCrafting::Clone(void* pArg)
{
	CCrafting* pInstance = new CCrafting(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCrafting");
		Safe_Release(pInstance);
	}

	CUI_Mgr::Get_Instance()->Set_Crafting(pInstance);

	return pInstance;
}

void CCrafting::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}
