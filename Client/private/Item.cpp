#include "Item.h"
#include "Inventory_Mgr.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CItem::CItem(CItem& Prototype)
    : CUIObject(Prototype)
{
}


HRESULT CItem::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
    CItem* pItem = this;

    m_iSlotIndex = (int*)pArg;
	m_iSlotIndexNum = *m_iSlotIndex;
    Desc.fSizeX = 44;
    Desc.fSizeY = 44;

	if (m_iSlotIndexNum < 9)
	{
		Desc.fY = g_iWinSizeY * 0.957f;
	}
	else
	{
		Desc.fY = g_iWinSizeY * 0.741f;
	}
	
	/* 하단 퀵슬롯 인벤토리 */
    if (m_iSlotIndexNum == 0) { Desc.fX = g_iWinSizeX * 0.293f; m_ItemType = ITEMNAME_WOOD;}
    if (m_iSlotIndexNum == 1) { Desc.fX = g_iWinSizeX * 0.346f; }
    if (m_iSlotIndexNum == 2) { Desc.fX = g_iWinSizeX * 0.397f; }
    if (m_iSlotIndexNum == 3) { Desc.fX = g_iWinSizeX * 0.450f; }
    if (m_iSlotIndexNum == 4) { Desc.fX = g_iWinSizeX * 0.502f; }
    if (m_iSlotIndexNum == 5) { Desc.fX = g_iWinSizeX * 0.554f; }
    if (m_iSlotIndexNum == 6) { Desc.fX = g_iWinSizeX * 0.604f; }
    if (m_iSlotIndexNum == 7) { Desc.fX = g_iWinSizeX * 0.655f; }
    if (m_iSlotIndexNum == 8) { Desc.fX = g_iWinSizeX * 0.707f; }

    /* 메인 인벤토리 */
    if (m_iSlotIndexNum == 9)   { Desc.fX = g_iWinSizeX * 0.336; /*m_ItemType = ITEMNAME_WOOD; */}
    if (m_iSlotIndexNum == 10)  { Desc.fX = g_iWinSizeX * 0.666; }


    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CItem::Priority_Update(_float fTimeDelta)
{
   
}

void CItem::Update(_float fTimeDelta)
{
 

}

void CItem::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CItem::Render()
{
  
	if (FAILED(m_pTextureCom->Bind_Resource(m_ItemType)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	__super::Begin();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End();


	return S_OK;
}

HRESULT CItem::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Item"), TEXT("Com_Texture"),
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

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CItem* pInstance = new CItem(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CItem");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CItem::Clone(void* pArg)
{
    CItem* pInstance = new CItem(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CItem");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CItem::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
\