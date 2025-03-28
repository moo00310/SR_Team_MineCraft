#include "Item.h"
#include "UI_Mgr.h"

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
    m_iSlotIndex = (int*)pArg;
	m_iSlotIndexNum = *m_iSlotIndex;
    Desc.fSizeX = 42.f;
    Desc.fSizeY = 42.f;

    /* 하단 퀵슬롯 인벤토리 */
	if (m_iSlotIndexNum < 9)
	{
        Desc.fX = 360.f + (m_iSlotIndexNum) * 70.f;
        Desc.fY = 672.f;
	}
    /* 메인 인벤토리 */
	else
	{
        Desc.fX = 407.f + (m_iSlotIndexNum - 9) * 58.f;
        Desc.fY = 510.f;
	}

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
    /* 메인 인벤토리 활성화일때만 렌더*/
    if (m_iSlotIndexNum >= 9 && !g_bMainInventoryOpen)
	{
		return S_OK;
	}

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

    CUI_Mgr::Get_Instance()->Add_Item(pInstance);

    return pInstance;
}

void CItem::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
