#include "Item.h"
#include "UI_Mgr.h"
#include "Mouse.h"

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

    /* �ϴ� ������ �κ��丮 */
	if (m_iSlotIndexNum < 9)
	{
        Desc.fX = 360.f + (m_iSlotIndexNum) * 70.f;
        Desc.fY = 672.f;
	}
    /* ���� �κ��丮 */
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
    if (m_iSlotIndexNum >= 9 && !g_bMainInventoryOpen)
        return;
  
    CMouse* pMouse = CMouse::Get_Instance();

    /* ���콺�� ������ ���� �� �浹 üũ */
    RECT rcRect;
    SetRect(&rcRect, (int)(Desc.fX - Desc.fSizeX * 0.5f), (int)(Desc.fY - Desc.fSizeY * 0.5f),
        (int)(Desc.fX + Desc.fSizeX * 0.5f), (int)(Desc.fY + Desc.fSizeY * 0.5f));

    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    CUI_Mgr* pUI_Mgr = CUI_Mgr::Get_Instance();

    auto mouse = pUI_Mgr->Get_MouseItemlist()->begin();

    /* ���콺 Ŭ�� �� ������ ���� �� ��ü */
    /* ���콺�� ������ ���� ���� �ְ�, ��Ŭ���� �������� �� ����*/
    if (PtInRect(&rcRect, ptMouse) && m_pGameInstance->Key_Up(VK_LBUTTON) && m_iSlotIndexNum >= 9)
    {
        /* ���콺�� �������� ������ => ���� ������ �������� ���� */
        if (pMouse->Get_Picked() == false)
        {
            /* ������ Ÿ�� ����*/
            pMouse->Set_ItemID(m_ItemID);
            /* ������ �ؽ��� ��ȣ ����*/
            pMouse->Set_ItemName(m_ItemName);
            /* �������� ���� �ִ� �κ��丮 ���� ��ȣ ����*/
            pMouse->Set_SlotIndex(m_iSlotIndexNum);
            /* ���콺�� �������� ��� �ִ� ���·� ���� */
            pMouse->Set_Picked(true);
            /* ���� ���Կ� �ִ� ������ ���� ���� */
            pMouse->Set_ItemCount(m_iItemCount);

            /* ���콺�� �������� ��� �ִ� ���·� ����*/
            (*mouse)->Set_Check(true);
            /* ���콺�� ǥ���� ������ �̹��� ���� */
            (*mouse)->Set_ItemName(m_ItemName);

            Set_ItemName(ITEMNAME_END);
        }
        /* �̹� �������� �ִٸ� => ���԰� ���콺�� �������� ��ȯ */
        else
        {   
            /* ���� ���������� Ȯ��  => ���� ��ġ�� ��� (���� ó��) */
            if (pMouse->Get_SlotIndex() == m_iSlotIndexNum)
                pMouse->Set_ItemMatch(true);

            else
                pMouse->Set_ItemMatch(false);

            ///* �̹� ������ �������� �ִٸ� ��ü ���� */
            if (pUI_Mgr->Get_vecItemlist()->at(m_iSlotIndexNum)->Get_ItemName() < ITEMNAME_END)
            {
                return;
            }

            /* �ش� ĭ�� �������� ���� ��� ���콺�� ����ִ� �����۰� ��ü  (�����ؾ���)*/
            /*if (pMouse->Get_ItemName() < ITEMNAME_END)
            {
                
            }*/
           
			/* ���� ������ ���� */
			pMouse->Set_OldItem(m_ItemName);
			pMouse->Set_OldItemCount(m_iItemCount);

			/* ���콺�� �ִ� �������� ���Ծ� ��ġ */
			m_ItemName = (pMouse->Get_ItemName());
			/* ���콺�� ��� �ִ� ������ ������ ������ ������ ������ ���� */
			Set_ItemCount(pMouse->Get_ItemCount());
			/* ���콺�� �������� ������������ ���� */
			pMouse->Set_Picked(false);
			/* ���콺 ���� ���� */
			m_bCheck = true;

			/* ���콺�� �� �̻� �������� ��� ���� �ʵ��� ���� = �� ����*/
			pMouse->Set_ItemID(ITEMID_END);
			/* ���콺�� �̹����� ITEMID_END�� ���� = ���콺�� ��� �ִ� �������� ���*/
			(*mouse)->Set_ItemName(ITEMNAME_END);
			/* ���콺 ���� ��Ȱ��ȭ */
			//(*mouse)->Set_Check(false);

			/* ���� ������ �̹��� ���� */
            if (pMouse->Get_SlotIndex() != m_iSlotIndexNum)
                pUI_Mgr->Get_vecItemlist()->at(pMouse->Get_SlotIndex())->Set_ItemName(ITEMNAME_END);
			
            /* �׽�Ʈ �Լ� Ȱ�� */
			pUI_Mgr->Get_vecItemlist()->at(pMouse->Get_SlotIndex())->Set_Test(true);
        }
    }

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CItem::Render()
{
    if (m_bCheck)
    {
        /* ���� �κ��丮 Ȱ��ȭ�϶��� ����*/
        if (m_iSlotIndexNum >= 9 && !g_bMainInventoryOpen)
        {
            return S_OK;
        }

        if (FAILED(m_pTextureCom->Bind_Resource(m_ItemName)))
            return E_FAIL;

        if (FAILED(m_pVIBufferCom->Bind_Buffers()))
            return E_FAIL;

        if (FAILED(m_pTransformCom->Bind_Resource()))
            return E_FAIL;

        __super::Begin();

        if (FAILED(m_pVIBufferCom->Render()))
            return E_FAIL;

        __super::End();
    }

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
