#include "Mouse_Item.h"
#include "UI_Mgr.h"
#include "Mouse.h"

CMouse_Item::CMouse_Item(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUIObject{ pGraphic_Device }
{
}

CMouse_Item::CMouse_Item(CMouse_Item& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CMouse_Item::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMouse_Item::Initialize(void* pArg)
{

    Desc.fSizeX = 42.f;
    Desc.fSizeY = 42.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(Desc.fX - g_iWinSizeX * 0.5f, -Desc.fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CMouse_Item::Priority_Update(_float fTimeDelta)
{
}

void CMouse_Item::Update(_float fTimeDelta)
{
    if (m_bCheck)
    {
        __super::Update(fTimeDelta);

        RECT rcRect;
        SetRect(&rcRect, (int)(Desc.fX - Desc.fSizeX * 0.5f), (int)(Desc.fY - Desc.fSizeY * 0.5f),
            (int)(Desc.fX + Desc.fSizeX * 0.5f), (int)(Desc.fY + Desc.fSizeY * 0.5f));

        POINT		ptMouse;
        GetCursorPos(&ptMouse);
        ScreenToClient(g_hWnd, &ptMouse);


        Desc.fX = (float)ptMouse.x;
        Desc.fY = (float)ptMouse.y;

        m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(Desc.fX - g_iWinSizeX * 0.5f, -Desc.fY + g_iWinSizeY * 0.5f, 0.f));
    }
}

void CMouse_Item::Late_Update(_float fTimeDelta)
{
    CMouse* pMouse = CMouse::Get_Instance();

    /* 아이템 종류에 따라 아이템 아이디 분류 */
    if (m_ItemName == ITEMNAME_WOOD)
    {
        pMouse->Set_ItemID(ITEMID_BLOCK);
    }

    if (m_bCheck)
    {
        __super::Late_Update(fTimeDelta);

        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
            return;
    }
}

HRESULT CMouse_Item::Render()
{
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

    return S_OK;
}

HRESULT CMouse_Item::Ready_Components()
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

HRESULT CMouse_Item::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 40);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CMouse_Item::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

CMouse_Item* CMouse_Item::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CMouse_Item* pInstance = new CMouse_Item(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CMouse_Item");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMouse_Item::Clone(void* pArg)
{
    CMouse_Item* pInstance = new CMouse_Item(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CMouse_Item");
        Safe_Release(pInstance);
    }

    CUI_Mgr::Get_Instance()->Get_MouseItemlist()->push_back(pInstance);

    return pInstance;
}

void CMouse_Item::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
