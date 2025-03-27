#include "Inventory_Back.h"

CInventory_Back::CInventory_Back(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUIObject{ pGraphic_Device }
{
}

CInventory_Back::CInventory_Back(CInventory_Back& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CInventory_Back::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory_Back::Initialize(void* pArg)
{
	m_iSlotIndex = (int*)pArg;
	m_iSlotIndexNum = *m_iSlotIndex;

	/* 하단 퀵슬롯 */
	if (m_iSlotIndexNum < 9)
	{
		m_iTextureNum = 0;
		Desc.fSizeX = 60.f;
		Desc.fSizeY = 75.f;
		Desc.fY = 672.f;
	}
	/* 메인 인벤토리 */
	else
	{
		m_iTextureNum = 1;
		Desc.fSizeX = 46.f;
		Desc.fSizeY = 49.f;
		Desc.fY = 510.f;
	}

	/* 하단 인벤토리 */
	if (m_iSlotIndexNum == 0) { Desc.fX = 360.f; }
	if (m_iSlotIndexNum == 1) { Desc.fX = 430.f; }
	if (m_iSlotIndexNum == 2) { Desc.fX = 500.f; }
	if (m_iSlotIndexNum == 3) { Desc.fX = 570.f; }
	if (m_iSlotIndexNum == 4) { Desc.fX = 640.f; }
	if (m_iSlotIndexNum == 5) { Desc.fX = 710.f; }
	if (m_iSlotIndexNum == 6) { Desc.fX = 780.f; }
	if (m_iSlotIndexNum == 7) { Desc.fX = 850.f; }
	if (m_iSlotIndexNum == 8) { Desc.fX = 920.f; }

	/* 메인 인벤토리 */
	if (m_iSlotIndexNum == 9)	{ Desc.fX = 428.f; }
	if (m_iSlotIndexNum == 10)	{ Desc.fX = 481.f; }
	if (m_iSlotIndexNum == 11) { Desc.fX = 534.f; }
	if (m_iSlotIndexNum == 12) { Desc.fX = 587.f; }
	if (m_iSlotIndexNum == 13) { Desc.fX = 640.f; }
	if (m_iSlotIndexNum == 14) { Desc.fX = 693.f; }
	if (m_iSlotIndexNum == 15) { Desc.fX = 746.f; }
	if (m_iSlotIndexNum == 16) { Desc.fX = 799.f; }
	if (m_iSlotIndexNum == 17) { Desc.fX = 852.f; }


	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	return S_OK;
}

void CInventory_Back::Priority_Update(_float fTimeDelta)
{
}

void CInventory_Back::Update(_float fTimeDelta)
{
}

void CInventory_Back::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CInventory_Back::Render()
{
	/*if (FAILED(m_pTextureCom->Bind_Resource(m_iTextureNum)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	__super::Begin();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End();*/

	return S_OK;
}

HRESULT CInventory_Back::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_InventoryBack"), TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CInventory_Back* CInventory_Back::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CInventory_Back* pInstance = new CInventory_Back(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInventory_Back");
		Safe_Release(pInstance);
	}
	return pInstance;

}

CGameObject* CInventory_Back::Clone(void* pArg)
{
	CInventory_Back* pInstance = new CInventory_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CInventory_Back");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInventory_Back::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}
