#include "SlotInfo.h"
#include "UI_Mgr.h"
#include "Mouse.h"

CSlotInfo::CSlotInfo(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CSlotInfo::CSlotInfo(CSlotInfo& Prototype)
    : CUIObject(Prototype)
{
}


HRESULT CSlotInfo::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSlotInfo::Initialize(void* pArg)
{
    m_iSlotIndex = (int*)pArg;
    m_iSlotIndexNum = *m_iSlotIndex;

    m_fOffsetX = 18.f;

    static UIOBJECT_DESC slotTable[] = {
        {0, 42.f, 42.f, 360.f, 672.f},  // 0~8 (퀵슬롯)
        {0, 42.f, 42.f, 408.f, 510.f},  // 9~17
        {0, 42.f, 42.f, 408.f, 442.f},  // 18~26
        {0, 42.f, 42.f, 408.f, 386.f},  // 27~35
        {0, 42.f, 42.f, 408.f, 329.f},  // 36~44
        {0, 42.f, 42.f, 407.f, 90.f},   // 45~48
        {0, 159.f, 223.f, 518.f, 173.f},// 49 (특수 슬롯)
        {0, 42.f, 42.f, 630.f, 258.f},  // 50
        {0, 42.f, 42.f, 697.f, 121.f},  // 51~52
        {0, 42.f, 42.f, 697.f, 177.f},  // 53~54
        {0, 42.f, 42.f, 878.f, 152.f}   // 55~
    };

    if (m_iSlotIndexNum < 9) m_iCategory = 0;
    else if (m_iSlotIndexNum < 18) m_iCategory = 1;
    else if (m_iSlotIndexNum < 27) m_iCategory = 2;
    else if (m_iSlotIndexNum < 36) m_iCategory = 3;
    else if (m_iSlotIndexNum < 45) m_iCategory = 4;
    else if (m_iSlotIndexNum < 49) { m_iCategory = 5; m_ItemID = ITEMID_ARMOR; }
    else if (m_iSlotIndexNum == 49) m_iCategory = 6;
    else if (m_iSlotIndexNum == 50) m_iCategory = 7;
    else if (m_iSlotIndexNum < 53) m_iCategory = 8;
    else if (m_iSlotIndexNum < 55) m_iCategory = 9;
    else m_iCategory = 10;

    // 공통 속성 할당
    Desc.fSizeX = slotTable[m_iCategory].fSizeX;
    Desc.fSizeY = slotTable[m_iCategory].fSizeY;
    Desc.fX = slotTable[m_iCategory].fX;
    Desc.fY = slotTable[m_iCategory].fY;

    // 위치 보정
    if (m_iCategory == 0)
        Desc.fX += m_iSlotIndexNum * 70.f;
    else if (m_iCategory >= 1 && m_iCategory <= 4)
        Desc.fX += (m_iSlotIndexNum - (m_iCategory * 9)) * 58.f;
    else if (m_iCategory == 5)
        Desc.fY += (m_iSlotIndexNum - 45) * 56.f;
    else if (m_iCategory == 8 || m_iCategory == 9)
        Desc.fX += (m_iSlotIndexNum - (m_iCategory == 8 ? 51 : 53)) * 58.f;


    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pItemTexture_TransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pItemTexture_TransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CSlotInfo::Priority_Update(_float fTimeDelta)
{
   
}

void CSlotInfo::Update(_float fTimeDelta)
{
    m_iTensDigit = m_iItemCount / 10;
    m_iOnesDigit = m_iItemCount % 10;
}

void CSlotInfo::Late_Update(_float fTimeDelta)
{
	/* 인벤토리 활성화되어있을때만 마우스 피킹 가능*/
	if (m_iSlotIndexNum >= 9 && !g_bMainInventoryOpen)
	{
		return;
	}

	/* 아이템 정보 저장*/
	CMouse* pMouse = CMouse::Get_Instance();
	CUI_Mgr* pUI_Mgr = CUI_Mgr::Get_Instance();

	auto mouseItem = pUI_Mgr->Get_MouseItemlist()->begin();
	auto mouseItemFont = pUI_Mgr->Get_MouseItemFontlist()->begin();

	/* 마우스와 아이템 슬롯 간 충돌 체크 */
	RECT rcRect;
	SetRect(&rcRect, (int)(Desc.fX - Desc.fSizeX * 0.5f), (int)(Desc.fY - Desc.fSizeY * 0.5f),
		(int)(Desc.fX + Desc.fSizeX * 0.5f), (int)(Desc.fY + Desc.fSizeY * 0.5f));

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	/* 마우스 클릭 시 아이템 선택 및 교체 */
	/* 마우스가 아이템 슬롯 위에 있고, 좌클릭이 떼어졌을 때 실행*/
	if (PtInRect(&rcRect, ptMouse) && m_pGameInstance->Key_Up(VK_LBUTTON))
	{
		/* 마우스에 아이템이 없으면 => 처음 실행 */
		if (pMouse->Get_Picked() == false)
		{
			if (m_ItemName != ITEMNAME_END)
			{
				/* 클릭한 슬롯의 아이템 및 폰트 저장*/
		        /* 1. 아이템 타입 저장*/
                 pMouse->Set_ItemID(m_ItemID);
				/* 2. 아이템 텍스쳐 번호 저장*/
				pMouse->Set_ItemName(m_ItemName);
				/* 3. 아이템이 원래 있던 인벤토리 슬롯 번호 저장*/
				pMouse->Set_SlotIndex(m_iSlotIndexNum);
				/* 4. 마우스가 아이템을 들고 있는 상태로 변경 */
				pMouse->Set_Picked(true);
				/* 5. 현재 슬롯에 있는 아이템 개수 저장 */
				pMouse->Set_ItemCount(m_iItemCount);

				/* 6. 마우스가 아이템을 들고 있는 상태로 저장*/
				(*mouseItem)->Set_Check(true);
				/* 마우스에 표시할 아이템 이미지 설정 */
				(*mouseItem)->Set_ItemName(m_ItemName);

				/* 마우스에 표시할 아이템 개수 이미지 설정 (CMouse_ItemFont 클래스에서 렌더)*/
				if (m_bCountRender && m_iTensDigit != 0 || m_iOnesDigit != 0)
				{
					/* 마우스가 아이템 개수를 들고 있는 상태로 저장*/
					(*mouseItemFont)->Set_Check(true);
					(*mouseItemFont)->Set_ItemFont_Tens(m_iTensDigit);
					(*mouseItemFont)->Set_ItemFont_Ones(m_iOnesDigit);
				}

				/* 마우스가 집었던 슬롯 아이템, 개수, 개수 렌더 비활성 */
				Set_ItemName(ITEMNAME_END);
				m_iItemCount = 0;
				m_bCountRender = false;
			}

		}

		/* 마우스에 이미 아이템이 있다면 */
		else
		{
            if (pMouse->Get_ItemID() != ITEMID_ARMOR)
            {
                if (m_ItemID == ITEMID_ARMOR)
                {
                    return;
                }
            }

			/* 같은 아이템인지 확인  => 개수 합치는 방식 (스택 처리) */
			/* pMouse->Get_SlotIndex() = 마우스가 들고 있는 아이템이 원래있던 인덱스
			   m_iSlotIndexNum = 새로 놓을곳의 인덱스 */
			if (pMouse->Get_ItemName() == m_ItemName)
			{
				pMouse->Set_ItemMatch(true);
                /* 합을 확인해서 64가 넘어가면 합치기 X
                   그게 아니면 마우스에 아이템 정보들 넘겨주기*/
                _int iTotalCount = pMouse->Get_ItemCount() + m_iItemCount;

                if (m_iItemCount >= 64)
                {
                    return;
                }

                if(pMouse->Get_ItemCount() == 64)
                {
                    _int iTempCount = m_iItemCount;
                    m_iItemCount = pMouse->Get_ItemCount();
                    pMouse->Set_ItemCount(iTempCount);
                    (*mouseItemFont)->Set_ItemFont_Tens(m_iTensDigit); 
                    (*mouseItemFont)->Set_ItemFont_Ones(m_iOnesDigit);
                }
			}

			/* 다른 아이템이면 */
            else
            {
                pMouse->Set_ItemMatch(false);

                if (m_ItemName != ITEMNAME_END && pMouse->Get_ItemName() != m_ItemName)
                {
                    /* 이전 슬롯과 현재 슬롯 아이템 교환*/
                    ITEMID eTempItemID = m_ItemID;
                    ITEMNAME eTempItemName = m_ItemName;                    
                    _int iTempCount = m_iItemCount;

                    /* 마우스에는 선택한 슬롯에 아이템 이미지와 아이템 개수 전달*/
                    (*mouseItemFont)->Set_Check(true);
                    (*mouseItem)->Set_ItemName(eTempItemName);
                    (*mouseItemFont)->Set_ItemFont_Tens(m_iTensDigit);
                    (*mouseItemFont)->Set_ItemFont_Ones(m_iOnesDigit);

                    // 선택한 슬롯의 아이템 정보를 마우스가 들고있던 정보들로 교체
                    pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndexNum)->Set_ItemName(pMouse->Get_ItemName());
                    pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndexNum)->Set_ItemCount(pMouse->Get_ItemCount());
                    //pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndexNum)->Set_ItemID(pMouse->Get_ItemID());


                    pMouse->Set_ItemName(eTempItemName);
                    pMouse->Set_ItemCount(iTempCount);
                    //pMouse->Set_ItemID(eTempItemID);
                }

                /* 이동한 슬롯의 아이템 개수 렌더 활성*/
                m_bCountRender = true;              
            }

            if (pUI_Mgr->Get_vecSlotInfolist()->at(m_iSlotIndexNum)->Get_ItemName() < ITEMNAME_END)
            {
                return;
            }

           /* 마우스 상태 */
           /* 마우스에 있던 아이템을 슬롯애 배치 */
			m_ItemName = (pMouse->Get_ItemName());
			/* 마우스가 들고 있던 아이템 개수를 슬롯의 아이템 개수로 설정 */
			Set_ItemCount(pMouse->Get_ItemCount());
			/* 마우스가 아이템을 내려놓음으로 변경 */
			pMouse->Set_Picked(false);
			/* 마우스 상태 갱신 */
			m_bCheck = true;
			/* 마우스가 더 이상 아이템을 들고 있지 않도록 변경 = 빈 상태*/
            if (pMouse->Get_ItemID() != ITEMID_ARMOR)
                pMouse->Set_ItemID(ITEMID_END);
			/* 마우스의 이미지를 ITEMID_END로 변경 = 마우스가 들고 있는 아이템을 비움*/
			(*mouseItem)->Set_ItemName(ITEMNAME_END);
			/* 마우스 상태 비활성화 */
			(*mouseItem)->Set_Check(false);
			(*mouseItemFont)->Set_Check(false);
		}
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CSlotInfo::Render()
{
    /* 인덱스가 9보다 큰데 인벤토리가 오픈 활성상태 아니면 렌더 안함*/
    if (m_iSlotIndexNum >= 9 && !g_bMainInventoryOpen)
    {
        return S_OK;
    }

    if (m_bCheck)
    {
        /* 아이템 이미지 렌더*/
        if (FAILED(RenderItemTexture(m_pItem_TextureCom, m_ItemName)))
            return E_FAIL;

        /* 아이템 텍스쳐와 개수 텍스쳐 사이즈 다르게 처리 */
        /* 여기서 0, 0 이 화면에 계속 출력되고있는 상태 */

        if (m_bCountRender)
        {
            /* 10의 자리 아이템 개수 렌더 */
            if (FAILED(RenderItemCount(m_pItemCount_TextureCom, m_iTensDigit, Desc.fX, Desc.fY, 14.f, 14.f)))
                return E_FAIL;

            /* 1의 자리 아이템 개수 렌더 */
            if (FAILED(RenderItemCount(m_pItemCount_TextureCom, m_iOnesDigit, Desc.fX - m_fOffsetX, Desc.fY, 14.f, 14.f)))
                return E_FAIL;
        }
    }

    return S_OK;
}

	
HRESULT CSlotInfo::RenderItemTexture(CTexture* pTextureCom, _int _TextureNum)
{
    int TextureIndex = -1;
	if (_TextureNum < 100)
		TextureIndex = _TextureNum;
	else if (_TextureNum < 200)
		TextureIndex = (_TextureNum - 100) + ITEMNAME_CUBE_END;

    if (FAILED(m_pItem_TextureCom->Bind_Resource(TextureIndex)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pItemTexture_TransformCom->Bind_Resource()))
        return E_FAIL;

    __super::Begin();
    SetUp_RenderState();

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    __super::End();
    Reset_RenderState();

	return S_OK;
}

HRESULT CSlotInfo::RenderItemCount(CTexture* pTextureCom, _int _TextureNum, _float _fX, _float _fY, _float _fsizeX, _float _fsizeY)
{
    _fX += 15.f;
    _fY += 12.f;

    if (FAILED(m_pItemCount_TextureCom->Bind_Resource(_TextureNum)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pItemCountTexture_TransformCom->Bind_Resource()))
        return E_FAIL;

    __super::Begin();
    //SetUp_RenderState();

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pItemCountTexture_TransformCom->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    m_pShaderCom->Bind_Texture("g_Texture", m_pItemCount_TextureCom->Get_Texture(_TextureNum));
    m_pShaderCom->Begin(3);

    /* 크기 및 위치 조정 */
    m_pItemCountTexture_TransformCom->Scaling(_fsizeX, _fsizeY, 1.f);
    m_pItemCountTexture_TransformCom->Set_State(CTransform::STATE_POSITION, _float3(_fX - g_iWinSizeX * 0.5f, -_fY + g_iWinSizeY * 0.5f, 0.f));


    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pShaderCom->End();

    __super::End();
    //Reset_RenderState();

    return S_OK;
}

HRESULT CSlotInfo::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CSlotInfo::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

void CSlotInfo::TextureNum_Update(_int _TextureNum)
{
    int TextureIndex = -1;
    if (_TextureNum < 100)
        TextureIndex = _TextureNum;
    else if (_TextureNum < 200)
        TextureIndex = (_TextureNum - 100) + ITEMNAME_CUBE_END;

    m_ItemName = static_cast<ITEMNAME>(TextureIndex);
}

HRESULT CSlotInfo::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Item"), TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pItem_TextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Font"), TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pItemCount_TextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pItemTexture_TransformCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pItemCountTexture_TransformCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

CSlotInfo* CSlotInfo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CSlotInfo* pInstance = new CSlotInfo(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CItem");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CSlotInfo::Clone(void* pArg)
{
    CSlotInfo* pInstance = new CSlotInfo(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CItem");
        Safe_Release(pInstance);
    }

    CUI_Mgr::Get_Instance()->Add_SlotUpdate(pInstance);

    return pInstance;
}

void CSlotInfo::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pItem_TextureCom);
    Safe_Release(m_pItemCount_TextureCom);
    Safe_Release(m_pItemTexture_TransformCom);
    Safe_Release(m_pItemCountTexture_TransformCom);
    Safe_Release(m_pShaderCom); 
}
