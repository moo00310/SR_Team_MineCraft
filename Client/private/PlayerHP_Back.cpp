#include "PlayerHP_Back.h"
#include "UI_Mgr.h"

CPlayerHP_Back::CPlayerHP_Back(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CPlayerHP_Back::CPlayerHP_Back(CPlayerHP_Back& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CPlayerHP_Back::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerHP_Back::Initialize(void* pArg)
{
    m_iHpCount = (int*)pArg;
    m_iHpIndex = *m_iHpCount;

    Desc.fSizeX = 30.f;
    Desc.fSizeY = 30.f;
    Desc.fX = 330.f + (m_iHpIndex * Desc.fSizeX);
    Desc.fY = 570.f;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CPlayerHP_Back::Priority_Update(_float fTimeDelta)
{
}

void CPlayerHP_Back::Update(_float fTimeDelta)
{
}

void CPlayerHP_Back::Late_Update(_float fTimeDelta)
{
    m_fTime = fTimeDelta;

    if (CUI_Mgr::Get_Instance()->Get_vecPlayerHPlist()->at(m_iHpIndex)->Get_Shake())
    {
        m_fShakeTime += fTimeDelta;

        if (m_fShakeTime >= 1.f)
        {
            CUI_Mgr::Get_Instance()->Get_vecPlayerHPlist()->at(m_iHpIndex)->Set_Shake(false);
            m_fShakeTime = 0.f;
        }
    }

    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CPlayerHP_Back::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    __super::Begin();
    SetUp_RenderState();

    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    m_pShaderCom->Bind_Texture("g_Texture", m_pTextureCom->Get_Texture(0));

    if (CUI_Mgr::Get_Instance()->Get_vecPlayerHPlist()->at(m_iHpIndex)->Get_Shake())
    {
        float fTimeValue = GetTickCount64() * 0.001f;
        m_pShaderCom->SetFloat("g_Time", fTimeValue);
        m_pShaderCom->SetFloat("g_ShakeStrength", 0.02f); // 필요 시 강도 조절

        m_pShaderCom->Begin(8);
    }
    else
    {
        m_pShaderCom->Begin(0);
    }

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pShaderCom->End();

    __super::End();
    Reset_RenderState();

    return S_OK;
}

HRESULT CPlayerHP_Back::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_PlayerHP"), TEXT("Com_Texture"),
        reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
        reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
        return E_FAIL;


    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerHP_Back::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CPlayerHP_Back::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

CPlayerHP_Back* CPlayerHP_Back::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayerHP_Back* pInstance = new CPlayerHP_Back(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPlayerHP_Back");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CPlayerHP_Back::Clone(void* pArg)
{
    CPlayerHP_Back* pInstance = new CPlayerHP_Back(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CPlayerHP_Back");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CPlayerHP_Back::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
}
