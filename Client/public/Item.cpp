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
            MSG_BOX("폰트 생성 실패");
        }
    }


    m_ItemMap = {
        {ITEMNAME::ITEMNAME_DIRT, L"흙"},
        {ITEMNAME::ITEMNAME_GRASSDIRT, L"잔디 블록"},
        {ITEMNAME::ITEMNAME_STONE, L"돌"},
        {ITEMNAME::ITEMNAME_COBBLESTONE, L"조약돌"},
        {ITEMNAME::ITEMNAME_WOOD, L"참나무 원목"},
        {ITEMNAME::ITEMNAME_OAKPLANKS, L"나무 판자"},
        {ITEMNAME::ITEMNAME_LEAF, L"참나무 잎"},
        {ITEMNAME::ITEMNAME_FURANCE, L"화로"},
        {ITEMNAME::ITEMNAME_CRAFTINGTABLE, L"제작대"},
        {ITEMNAME::ITEMNAME_STICK, L"막대기"},
        {ITEMNAME::ITEMNAME_APPLE, L"사과"},
        {ITEMNAME::ITEMNAME_SAPLING, L"묘묙"},
        {ITEMNAME::ITEMNAME_REDTULIP, L"빨간색 튤립"},
        {ITEMNAME::ITEMNAME_DANDELION, L"민들레"},
        {ITEMNAME::ITEMNAME_COAL, L"석탄"},
        {ITEMNAME::ITEMNAME_RAWIRON, L"철 원석"},
        {ITEMNAME::ITEMNAME_IRON, L"철 주괴"},
        {ITEMNAME::ITEMNAME_TORCH, L"횃불"},
        {ITEMNAME::ITEMNAME_GUNPOWDER, L"화약"},
        {ITEMNAME::ITEMNAME_ROTTENFLESH, L"썩은 살점"},
        {ITEMNAME::ITEMNAME_SEED, L"밀 씨앗"},
        {ITEMNAME::ITEMNAME_STONE_SWORD, L"돌 검"},
        {ITEMNAME::ITEMNAME_STONE_PICKAXE, L"돌 곡괭이"},
        {ITEMNAME::ITEMNAME_STONE_AXE, L"돌 도끼"},
        {ITEMNAME::ITEMNAME_STEEL_SWORD, L"철 검"},
        {ITEMNAME::ITEM_WEPON_1, L"거대한 붉은 검"},
        {ITEMNAME::ITEMNAME_WOOD_PICKAXE, L"나무 곡괭이"}
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
