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
	/* ������ ������ ���չ� ���� */
	RECIPE_DESC stick1 = { {{51, ITEMNAME_OAKPLANKS}, {53, ITEMNAME_OAKPLANKS}}, ITEMNAME_STICK, 1 };
	RECIPE_DESC stick2 = { {{52, ITEMNAME_OAKPLANKS}, {54, ITEMNAME_OAKPLANKS}}, ITEMNAME_STICK, 1 };
	
	m_vecRecipelist.push_back(stick1);
	m_vecRecipelist.push_back(stick2);

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
		_bool bMatch = true;

		for (const auto& pair : recipe.inputPattern)
		{
			/* ������ (���� �ε����� ������ ���)*/
			_int iSlotIndex = pair.first;
			ITEMNAME  requiredItem = pair.second;

			if (pUI_Mgr->Get_vecSlotInfolist()->at(iSlotIndex)->Get_ItemName() != requiredItem)
			{
				bMatch = false;
				break;
			}
		}

		if (bMatch)
		{
			for (const auto& pair : recipe.inputPattern)
			{
				int iSlotIndex = pair.first;
				
				pUI_Mgr->Get_vecSlotInfolist()->at(iSlotIndex)->Set_ItemName(ITEMNAME_END);
			}

	
			/* ��� �� */
			m_iItemCount /= 2;

			/* ������ ���� ��� �������� 55�� ���Կ� �ֱ� */
			pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemName(recipe.resultItem);
			pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCount(m_iItemCount);
			pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemCountRender(true);
			
			m_iItemCount = 0;
			//pUI_Mgr->Get_vecSlotInfolist()->at(55)->Set_ItemID(ITEMID_END);

			break;
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
