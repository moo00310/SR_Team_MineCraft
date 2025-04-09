#include "Item.h"
#include "UI_Mgr.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

CItem::CItem(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CItem::CItem(CItem& Prototype)
    : CUIObject( Prototype )
{
}

HRESULT CItem::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
    GetModuleFileName(nullptr, m_szPath, MAX_PATH);
    PathRemoveFileSpec(m_szPath);

    m_fontPath = _wstring(m_szPath) + L"\\Resources\\Fonts\\font.ttf";
    if (!AddFontResourceEx(m_fontPath.c_str(), FR_PRIVATE, 0))
    {
        MSG_BOX("Failed to Created : ItemFont");
        return E_FAIL;
    }

    if (m_pFont == nullptr)
    {
        D3DXFONT_DESC fontDesc = {};
        fontDesc.Height = 34;
        fontDesc.Width = 0;
        fontDesc.Weight = FW_NORMAL;
        fontDesc.CharSet = DEFAULT_CHARSET;
        fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
        fontDesc.Quality = CLEARTYPE_QUALITY;
        fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
        lstrcpy(fontDesc.FaceName, L"Galmuri7 Regular");

       
        if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &fontDesc, &m_pFont)))
        {
            MSG_BOX("ÆùÆ® »ý¼º ½ÇÆÐ");
        }
    }


    m_ItemMap = {
        {ITEMNAME::ITEMNAME_DIRT, L"Èë"},
        {ITEMNAME::ITEMNAME_GRASSDIRT, L"ÀÜµð ºí·Ï"},
        {ITEMNAME::ITEMNAME_STONE, L"µ¹"},
        {ITEMNAME::ITEMNAME_COBBLESTONE, L"Á¶¾àµ¹"},
        {ITEMNAME::ITEMNAME_WOOD, L"Âü³ª¹« ¿ø¸ñ"},
        {ITEMNAME::ITEMNAME_OAKPLANKS, L"³ª¹« ÆÇÀÚ"},
        {ITEMNAME::ITEMNAME_LEAF, L"Âü³ª¹« ÀÙ"},
        {ITEMNAME::ITEMNAME_FURANCE, L"È­·Î"},
        {ITEMNAME::ITEMNAME_CRAFTINGTABLE, L"Á¦ÀÛ´ë"},
        {ITEMNAME::ITEMNAME_STICK, L"¸·´ë±â"},
        {ITEMNAME::ITEMNAME_APPLE, L"»ç°ú"},
        {ITEMNAME::ITEMNAME_SAPLING, L"¹¦‘ò"},
        {ITEMNAME::ITEMNAME_REDTULIP, L"»¡°£»ö Æ«¸³"},
        {ITEMNAME::ITEMNAME_DANDELION, L"¹Îµé·¹"},
        {ITEMNAME::ITEMNAME_COAL, L"¼®Åº"},
        {ITEMNAME::ITEMNAME_RAWIRON, L"Ã¶ ¿ø¼®"},
        {ITEMNAME::ITEMNAME_IRON, L"Ã¶ ÁÖ±«"},
        {ITEMNAME::ITEMNAME_TORCH, L"È¶ºÒ"},
        {ITEMNAME::ITEMNAME_GUNPOWDER, L"È­¾à"},
        {ITEMNAME::ITEMNAME_ROTTENFLESH, L"½âÀº »ìÁ¡"},
        {ITEMNAME::ITEMNAME_SEED, L"¹Ð ¾¾¾Ñ"},
        {ITEMNAME::ITEMNAME_STONE_SWORD, L"µ¹ °Ë"},
        {ITEMNAME::ITEMNAME_STONE_PICKAXE, L"µ¹ °î±ªÀÌ"},
        {ITEMNAME::ITEMNAME_STONE_AXE, L"µ¹ µµ³¢"},
        {ITEMNAME::ITEMNAME_STEEL_SWORD, L"Ã¶ °Ë"},
        {ITEMNAME::ITEM_WEPON_1, L"°Å´ëÇÑ ºÓÀº °Ë"},
        {ITEMNAME::ITEMNAME_WOOD_PICKAXE, L"³ª¹« °î±ªÀÌ"}
    };

    UIOBJECT_DESC Desc{};
    
    Desc.fSizeX = 200.f;
    Desc.fSizeY = 70.f;
    Desc.fX = m_fX;
    Desc.fY = m_fY;
    
    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

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
    if (m_pFont != nullptr && m_bItemTextRender && *m_bItemTextRender)
    {

        if (FAILED(m_pTextureCom->Bind_Resource(0)))
            return E_FAIL;

        if (FAILED(m_pVIBufferCom->Bind_Buffers()))
            return E_FAIL;

        if (FAILED(m_pTransformCom->Bind_Resource()))
            return E_FAIL;

        __super::Begin();

        m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3((m_fX - g_iWinSizeX * 0.5f) + 120.f,  ( - m_fY + g_iWinSizeY * 0.5f) + 20.f, 0.f));

        if (FAILED(m_pVIBufferCom->Render()))
            return E_FAIL;

        __super::End();

        m_pFont->DrawTextW(
            NULL,
            m_strText.c_str(),
            -1,
            &textRect,
            DT_NOCLIP,
            D3DCOLOR_ARGB(255, 255, 255, 255)
        );
    }

    return S_OK;
}

void CItem::Clear_ItemTextRender()
{
    if(m_bItemTextRender)
        *m_bItemTextRender = false;

    m_bItemTextRender = nullptr;
    m_strText.clear();
    SetRect(&textRect, 0, 0, 0, 0);

}

void CItem::ItemMatch(ITEMNAME _ItemName, _bool* _bTestInfo, _float _fX, _float _fY)
{
    auto iter = m_ItemMap.find(_ItemName);

    if (iter != m_ItemMap.end())
        m_strText = iter->second;
    
    m_pFont;
    m_bItemTextRender = _bTestInfo;
    m_fX = _fX;
    m_fY = _fY;
    
    SetRect(&textRect, m_fX + 50.f, m_fY - 40.f, m_fX + 250.f, m_fY - 10.f);
}

HRESULT CItem::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_InfoBox"), TEXT("Com_Texture"),
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

    CUI_Mgr::Get_Instance()->Set_Item(pInstance);

    return pInstance;
}

void CItem::Free()
{
    if (m_pFont)
    {
        m_pFont->Release();
        m_pFont = nullptr;
    }

    RemoveFontResourceEx(m_fontPath.c_str(), FR_PRIVATE, 0);
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
