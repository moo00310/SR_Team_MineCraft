#include "Inventory_Back.h"
#include "UI_Mgr.h"

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

	static UIOBJECT_DESC  slotTable[] = {
	{0, 60.f, 75.f, 360.f, 672.f},  // 0~8 (Äü½½·Ô)
	{1, 53.f, 52.5f, 407.f, 508.f}, // 9~17
	{1, 53.f, 52.5f, 407.f, 440.f}, // 18~26
	{1, 53.f, 52.5f, 407.f, 384.f}, // 27~35
	{1, 53.f, 52.5f, 407.f, 327.f}, // 36~44
	{1, 53.f, 52.5f, 407.f, 90.f},  // 45~48
	{1, 159.f, 223.f, 518.f, 173.f}, // 49
	{1, 53.f, 52.5f, 630.f, 258.f},  // 50
	{1, 53.f, 52.5f, 697.f, 121.f},  // 51~52
	{1, 53.f, 52.5f, 697.f, 177.f},  // 53~54
	{1, 53.f, 52.5f, 878.f, 152.f}   // ±âÅ¸
	};

	if (m_iSlotIndexNum < 9) m_iCategory = 0;
	else if (m_iSlotIndexNum < 18) m_iCategory = 1;
	else if (m_iSlotIndexNum < 27) m_iCategory = 2;
	else if (m_iSlotIndexNum < 36) m_iCategory = 3;
	else if (m_iSlotIndexNum < 45) m_iCategory = 4;
	else if (m_iSlotIndexNum < 49) m_iCategory = 5;
	else if (m_iSlotIndexNum == 49) m_iCategory = 6;
	else if (m_iSlotIndexNum == 50) m_iCategory = 7;
	else if (m_iSlotIndexNum < 53) m_iCategory = 8;
	else if (m_iSlotIndexNum < 55) m_iCategory = 9;
	else m_iCategory = 10;

	m_iTextureNum = slotTable[m_iCategory].iTextureNum;
	Desc.fSizeX = slotTable[m_iCategory].fSizeX;
	Desc.fSizeY = slotTable[m_iCategory].fSizeY;
	Desc.fX = slotTable[m_iCategory].fX;
	Desc.fY = slotTable[m_iCategory].fY;

	if (m_iCategory == 0)
		Desc.fX += m_iSlotIndexNum * 70.f;
	else if (m_iCategory >= 1 && m_iCategory <= 4)
		Desc.fX += (m_iSlotIndexNum - (m_iCategory * 9)) * 58.f;
	else if (m_iCategory == 5)
		Desc.fY = 90.f + (m_iSlotIndexNum - 45) * 56.f;
	else if (m_iCategory == 8)
		Desc.fX += (m_iSlotIndexNum - 51) * 58.f;
	else if (m_iCategory == 9)
		Desc.fX += (m_iSlotIndexNum - 53) * 58.f;

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
	//if (FAILED(m_pTextureCom->Bind_Resource(m_iTextureNum)))
	//	return E_FAIL;

	//if (FAILED(m_pVIBufferCom->Bind_Buffers()))
	//	return E_FAIL;

	//if (FAILED(m_pTransformCom->Bind_Resource()))
	//	return E_FAIL;

	//__super::Begin();

	//if (FAILED(m_pVIBufferCom->Render()))
	//	return E_FAIL;

	//__super::End();

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

	//CUI_Mgr::Get_Instance()->Get_vecInventoryBacklist()->push_back(pInstance);

	return pInstance;
}

void CInventory_Back::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}
