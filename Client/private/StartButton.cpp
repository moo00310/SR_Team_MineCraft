#include "StartButton.h"
#include "UI_Mgr.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

CStartButton::CStartButton(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CStartButton::CStartButton(CStartButton& Prototype)
    : CUIObject( Prototype )
{
}

HRESULT CStartButton::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStartButton::Initialize(void* pArg)
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
        fontDesc.Height = 48;
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

    UIOBJECT_DESC Desc{};

    Desc.fSizeX = 550;
    Desc.fSizeY = 80;
    Desc.fX = g_iWinSizeX * 0.5f; // 640 
    Desc.fY = g_iWinSizeY * 0.5f + 220.f; // 580

    SetRect(&textRect,
        560.f,
        550.f, // 540 
        Desc.fX + (Desc.fSizeX * 0.5f),
        Desc.fY + (Desc.fSizeY * 0.5f));

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
    
 	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));


    if (m_pFont == nullptr)
        return E_FAIL;

    return S_OK;
}

void CStartButton::Priority_Update(_float fTimeDelta)
{
    swprintf(m_szBuffer, 128, L"게임 시작");
    m_strText = m_szBuffer;

    if (m_strText.empty())
        return;
}

void CStartButton::Update(_float fTimeDelta)
{
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    if (PtInRect(&textRect, ptMouse))
    {
        m_iTextureNum = 1;
    }
    else
        m_iTextureNum = 0;

    if (m_pGameInstance->Key_Down(VK_LBUTTON)) {
        if (true == __super::isPick(g_hWnd))
            g_bChangeLevel = true;
    }
}

void CStartButton::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CStartButton::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(m_iTextureNum)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    __super::Begin();

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

    return S_OK;
}

HRESULT CStartButton::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_StartButton"), TEXT("Com_Texture"),
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

CStartButton* CStartButton::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CStartButton* pInstance = new CStartButton(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CStartButton");
        Safe_Release(pInstance);
    }
    return pInstance;

}

CGameObject* CStartButton::Clone(void* pArg)
{
    CStartButton* pInstance = new CStartButton(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CStartButton");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CStartButton::Free()
{
    __super::Free();

    Safe_Release(m_pFont);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
}
