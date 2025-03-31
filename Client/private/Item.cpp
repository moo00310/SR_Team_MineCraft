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
    if (m_iSlotIndexNum >= 9 && !g_bMainInventoryOpen)
        return;
  
    CMouse* pMouse = CMouse::Get_Instance();

    /* 마우스와 아이템 슬롯 간 충돌 체크 */
    RECT rcRect;
    SetRect(&rcRect, (int)(Desc.fX - Desc.fSizeX * 0.5f), (int)(Desc.fY - Desc.fSizeY * 0.5f),
        (int)(Desc.fX + Desc.fSizeX * 0.5f), (int)(Desc.fY + Desc.fSizeY * 0.5f));

    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    CUI_Mgr* pUI_Mgr = CUI_Mgr::Get_Instance();

    auto mouse = pUI_Mgr->Get_MouseItemlist()->begin();

    /* 마우스 클릭 시 아이템 선택 및 교체 */
    /* 마우스가 아이템 슬롯 위에 있고, 좌클릭이 떼어졌을 때 실행*/
    if (PtInRect(&rcRect, ptMouse) && m_pGameInstance->Key_Up(VK_LBUTTON) && m_iSlotIndexNum >= 9)
    {
        /* 마우스에 아이템이 없으면 => 현재 슬롯의 아이템을 집음 */
        if (pMouse->Get_Picked() == false)
        {
            /* 아이템 타입 저장*/
            pMouse->Set_ItemID(m_ItemID);
            /* 아이템 텍스쳐 번호 저장*/
            pMouse->Set_ItemName(m_ItemName);
            /* 아이템이 원래 있던 인벤토리 슬롯 번호 저장*/
            pMouse->Set_SlotIndex(m_iSlotIndexNum);
            /* 마우스가 아이템을 들고 있는 상태로 변경 */
            pMouse->Set_Picked(true);
            /* 현재 슬롯에 있는 아이템 개수 저장 */
            pMouse->Set_ItemCount(m_iItemCount);

            /* 마우스가 아이템을 들고 있는 상태로 저장*/
            (*mouse)->Set_Check(true);
            /* 마우스에 표시할 아이템 이미지 설정 */
            (*mouse)->Set_ItemName(m_ItemName);

            Set_ItemName(ITEMNAME_END);
        }
        /* 이미 아이템이 있다면 => 슬롯과 마우스의 아이템을 교환 */
        else
        {   
            /* 같은 아이템인지 확인  => 개수 합치는 방식 (스택 처리) */
            if (pMouse->Get_SlotIndex() == m_iSlotIndexNum)
                pMouse->Set_ItemMatch(true);

            else
                pMouse->Set_ItemMatch(false);

            ///* 이미 슬롯의 아이템이 있다면 교체 못함 */
            if (pUI_Mgr->Get_vecItemlist()->at(m_iSlotIndexNum)->Get_ItemName() < ITEMNAME_END)
            {
                return;
            }

            /* 해당 칸에 아이템이 있을 경우 마우스가 들고있는 아이템과 교체  (수정해야함)*/
            /*if (pMouse->Get_ItemName() < ITEMNAME_END)
            {
                
            }*/
           
			/* 이전 아이템 저장 */
			pMouse->Set_OldItem(m_ItemName);
			pMouse->Set_OldItemCount(m_iItemCount);

			/* 마우스에 있던 아이템을 슬롯애 배치 */
			m_ItemName = (pMouse->Get_ItemName());
			/* 마우스가 들고 있던 아이템 개수를 슬롯의 아이템 개수로 설정 */
			Set_ItemCount(pMouse->Get_ItemCount());
			/* 마우스가 아이템을 내려놓음으로 변경 */
			pMouse->Set_Picked(false);
			/* 마우스 상태 갱신 */
			m_bCheck = true;

			/* 마우스가 더 이상 아이템을 들고 있지 않도록 변경 = 빈 상태*/
			pMouse->Set_ItemID(ITEMID_END);
			/* 마우스의 이미지를 ITEMID_END로 변경 = 마우스가 들고 있는 아이템을 비움*/
			(*mouse)->Set_ItemName(ITEMNAME_END);
			/* 마우스 상태 비활성화 */
			//(*mouse)->Set_Check(false);

			/* 슬롯 아이템 이미지 비우기 */
            if (pMouse->Get_SlotIndex() != m_iSlotIndexNum)
                pUI_Mgr->Get_vecItemlist()->at(pMouse->Get_SlotIndex())->Set_ItemName(ITEMNAME_END);
			
            /* 테스트 함수 활성 */
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
        /* 메인 인벤토리 활성화일때만 렌더*/
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
