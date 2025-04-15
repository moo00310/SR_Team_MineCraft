#include "WaveUi.h"
#include "MissionControl.h"

CWaveUi::CWaveUi(LPDIRECT3DDEVICE9 pGraphic_Device) :CUIObject{ pGraphic_Device }
{
}

CWaveUi::CWaveUi(CWaveUi& Prototype) :CUIObject(Prototype)
{
}

HRESULT CWaveUi::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWaveUi::Initialize(void* pArg)
{
    Desc.fSizeX = 150.f;
    Desc.fSizeY = 50.f;
    Desc.fX = 80;
    Desc.fY = 50;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}


void CWaveUi::Priority_Update(_float fTimeDelta)
{
}

void CWaveUi::Update(_float fTimeDelta)
{
}

void CWaveUi::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

struct CUSTOMVERTEX
{
    float x, y, z, rhw;  // 화면 좌표 + RHW (2D용)
    DWORD color;         // 색상
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

HRESULT CWaveUi::Render()
{
    CMissionControl* _control = dynamic_cast<CMissionControl*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Mission")));

    if (nullptr == _control)
        return E_FAIL;

    if (_control->Get_IsWaveStart()) {
        m_pGraphic_Device->SetTexture(0, nullptr);
        RECT rect2 = { 350, 0, 900, 100 };

        g_pWaveFont->DrawTextW(
            nullptr,
            L".. Wave Coming ..",
            -1,
            &rect2,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE,
            D3DCOLOR_ARGB(255, 255, 50, 59)
        );

        RECT rectGageBackground = { 350, 90, 900, 100 };
        DWORD fillColor = D3DCOLOR_ARGB(255, 0, 0, 0);

        CUSTOMVERTEX vertices[] =
        {
            { (float)rectGageBackground.left,  (float)rectGageBackground.top,    0.0f, 1.0f, fillColor },
            { (float)rectGageBackground.right, (float)rectGageBackground.top,    0.0f, 1.0f, fillColor },
            { (float)rectGageBackground.left,  (float)rectGageBackground.bottom, 0.0f, 1.0f, fillColor },
            { (float)rectGageBackground.right, (float)rectGageBackground.bottom, 0.0f, 1.0f, fillColor },
        };
        m_pGraphic_Device->SetFVF(D3DFVF_CUSTOMVERTEX);
        m_pGraphic_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(CUSTOMVERTEX));

        m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        LONG gageLeft = 350;
        LONG gageRight = gageLeft + (LONG)m_fGageMount;

        RECT rectGage = { gageLeft, 90, gageRight, 100 };
        DWORD fillColor2 = D3DCOLOR_ARGB(255, 60, 179, 113);

        m_fGageMount += 1.88f;
        m_fGageMount = min(m_fGageMount, 550.f);
        CUSTOMVERTEX vertices2[] =
        {
            { (float)rectGage.left,  (float)rectGage.top,    0.0f, 1.0f, fillColor2 },
            { (float)rectGage.right, (float)rectGage.top,    0.0f, 1.0f, fillColor2 },
            { (float)rectGage.left,  (float)rectGage.bottom, 0.0f, 1.0f, fillColor2 },
            { (float)rectGage.right, (float)rectGage.bottom, 0.0f, 1.0f, fillColor2 },
        };
        m_pGraphic_Device->SetFVF(D3DFVF_CUSTOMVERTEX);
        m_pGraphic_Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices2, sizeof(CUSTOMVERTEX));
    }

    if (_control->Get_IsWave()) {
        m_fGageMount = 0;
        if (FAILED(m_pTextureCom->Bind_Resource(0)))
            return E_FAIL;

        if (FAILED(m_pVIBufferCom->Bind_Buffers()))
            return E_FAIL;

        if (FAILED(m_pTransformCom->Bind_Resource()))
            return E_FAIL;

        __super::Begin();
        if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
            return E_FAIL;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
            return E_FAIL;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
            return E_FAIL;

        m_pShaderCom->Bind_Texture("g_Texture", m_pTextureCom->Get_Texture(0));

        m_pShaderCom->Begin(4);

        /* 정점을 그린다. */
        if (FAILED(m_pVIBufferCom->Render()))
            return E_FAIL;

        m_pShaderCom->End();
        __super::End();


        RECT rect = { 10, 0, 160, 100 };

        switch (_control->Get_CurrentStage())
        {
        case CMissionControl::WAVE1:
            g_pTitleFont->DrawTextW(
                nullptr,
                L"WAVE 1",
                -1,
                &rect,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                D3DCOLOR_ARGB(255, 255, 50, 59)
            );
            break;
        case CMissionControl::WAVE2:
            g_pTitleFont->DrawTextW(
                nullptr,
                L"WAVE 3",
                -1,
                &rect,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                D3DCOLOR_ARGB(255, 255, 50, 59)
            );
            break;
        case CMissionControl::WAVE3:
            g_pTitleFont->DrawTextW(
                nullptr,
                L"WAVE 3",
                -1,
                &rect,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                D3DCOLOR_ARGB(255, 255, 50, 59)
            );
            break;
        }

    }
    return S_OK;
}

HRESULT CWaveUi::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_WaveUi"), TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

CWaveUi* CWaveUi::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CWaveUi* pInstance = new CWaveUi(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CWaveUi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWaveUi::Clone(void* pArg)
{
    CWaveUi* pInstance = new CWaveUi(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CMissionMainUi");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CWaveUi::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
}
