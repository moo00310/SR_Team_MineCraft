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
	RECIPE_DESC oakplanks1 = { {{51, ITEMNAME_WOOD}}, ITEMNAME_OAKPLANKS, 1, 1 };
	RECIPE_DESC oakplanks2 = { {{52, ITEMNAME_WOOD}}, ITEMNAME_OAKPLANKS, 1, 1 };
	RECIPE_DESC oakplanks3 = { {{53, ITEMNAME_WOOD}}, ITEMNAME_OAKPLANKS, 1, 1 };
	RECIPE_DESC oakplanks4 = { {{54, ITEMNAME_WOOD}}, ITEMNAME_OAKPLANKS, 1, 1 };
	RECIPE_DESC stick1 = { {{51, ITEMNAME_OAKPLANKS}, {53, ITEMNAME_OAKPLANKS}}, ITEMNAME_STICK, 1, 2 };
	RECIPE_DESC stick2 = { {{52, ITEMNAME_OAKPLANKS}, {54, ITEMNAME_OAKPLANKS}}, ITEMNAME_STICK, 1, 2 };
	RECIPE_DESC CraftingTable = { {{51, ITEMNAME_OAKPLANKS}, {52, ITEMNAME_OAKPLANKS}, {53, ITEMNAME_OAKPLANKS}, {54, ITEMNAME_OAKPLANKS}}
									, ITEMNAME_CRAFTINGTABLE, 1, 4 };
	
	m_vecRecipelist.push_back(oakplanks1);
	m_vecRecipelist.push_back(oakplanks2);
	m_vecRecipelist.push_back(oakplanks3);
	m_vecRecipelist.push_back(oakplanks4);
	m_vecRecipelist.push_back(stick2);
	m_vecRecipelist.push_back(stick1);
	m_vecRecipelist.push_back(CraftingTable);

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
		if (m_bMatch)
		{
			/* 필요한 재료 개수 */
			m_iItemTotalCount = recipe.requiredItemCounts;
			/* 필요한 슬롯의 개수 합 0*/
			m_iSlotTotalCount = 0; 
			/* 완성 개수 */
			m_iresultCount = recipe.resultCount; 
			/* 재료가 존재하는 슬롯 인덱스 */
		
			for (const auto& pair : recipe.inputPattern)
			{
				m_iSlotIndex = pair.first;

				/* 슬롯에 들어간 아이템 개수 확인 */
				m_iTempCount = pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndex)->Get_ItemCount();
				m_iSlotTotalCount += m_iTempCount;

				/* 필요한 개수 마이너스 */
				m_iTempCount -= recipe.resultCount;

				if (m_iTempCount <= 0)
				{
					pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndex)->Set_ItemName(ITEMNAME_END);
				}
				else
				{
					pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndex)->Set_ItemCount(m_iTempCount);
				}
			}
			/* 레시피 조합 결과 아이템을 55번 슬롯에 넣기 */
			pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemName(recipe.resultItem);
			if (recipe.resultItem == ITEMNAME_OAKPLANKS)
			{
				pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCount(recipe.resultCount * 4);
			}
			else
			{
				pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCount(recipe.resultCount);
			}

			pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCountRender(true);
			//pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemID(ITEMID_END);
			break;
		}
	}
}

void CCrafting::RButton()
{
	CUI_Mgr* pUI_Mgr = CUI_Mgr::Get_Instance();

	if (pUI_Mgr->Get_vecSlotInfolist()->at(55)->Get_ItemCount() <= 0)
		return;

	int iTemp = pUI_Mgr->Get_vecSlotInfolist()->at(55)->Get_ItemCount();

	if (m_bMatch)
	{
		// m_iSlotTotalCount = 51 ~ 54번 슬롯 아이템 합				: 6개
		// m_iItemTotalCount = 레시피에 필요한 아이템 총 개수		: 4개
		// 슬롯 합 / 제작에 필요한 총 개수 = Test (제작된 개수)		:  6 / 4 = 1개 + 1 

		m_iSlotTotalCount -= m_iItemTotalCount;
		int Test = m_iSlotTotalCount / m_iItemTotalCount; 

		if (pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndex)->Get_ItemName() == ITEMNAME_WOOD)
		{
			/* 2 * 4 = 8 + at(55).count  */
			int iTemp = pUI_Mgr->Get_vecSlotInfolist()->at(55)->Get_ItemCount();
			pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCount(Test * 4 + iTemp);
		}
		else
		{
			pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCount(Test + 1);
		}
		
		pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCountRender(true);

		for (int i = 51; i < 55; ++i)
		{
			int iTempCount = pUI_Mgr->Get_vecSlotInfolist()->at(i)->Get_ItemCount();

			for (int j = 0; j < Test; ++j)
			{
				iTempCount -= m_iresultCount;
				pUI_Mgr->Get_vecSlotInfolist()->at(i)->Set_ItemCount(iTempCount);

				if (iTempCount == 0)
				{
					pUI_Mgr->Get_vecSlotInfolist()->at(i)->Set_ItemName(ITEMNAME_END);
					break;
				}
			}
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
