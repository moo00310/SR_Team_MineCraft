#include "PlayerHP.h"
#include "UI_Mgr.h"

CPlayerHP::CPlayerHP(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{ pGraphic_Device }
{
}

CPlayerHP::CPlayerHP(CPlayerHP& Prototype)
    : CUIObject( Prototype )
{
}

HRESULT CPlayerHP::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerHP::Initialize(void* pArg)
{
    m_iHpCount = (int*)pArg;
    m_iHpIndex = *m_iHpCount;

    m_iTextureNum = 1;
    Desc.fSizeX = 25.f;
    Desc.fSizeY = 25.f;
    Desc.fX = 330.f + (m_iHpIndex * 30.f);
    Desc.fY = 570.f;
   
    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CPlayerHP::Priority_Update(_float fTimeDelta)
{
}

void CPlayerHP::Update(_float fTimeDelta)
{

}

void CPlayerHP::Late_Update(_float fTimeDelta)
{

    m_fTime = fTimeDelta;

    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CPlayerHP::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(m_iTextureNum)))
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

    m_pShaderCom->Bind_Texture("g_Texture", m_pTextureCom->Get_Texture(m_iTextureNum));

    if (CUI_Mgr::Get_Instance()->Get_PlayerHP_Shader())
    {
        float timeValue = GetTickCount64() * 0.001f;
        m_pShaderCom->SetFloat("g_Time", timeValue);
        m_pShaderCom->Begin(2);
    }

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

    m_pShaderCom->End();

	__super::End();
    Reset_RenderState();

	return S_OK;
}

HRESULT CPlayerHP::Ready_Components()
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

HRESULT CPlayerHP::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 160);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT  CPlayerHP::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

CPlayerHP* CPlayerHP::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayerHP* pInstance = new CPlayerHP(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPlayerHP");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CPlayerHP::Clone(void* pArg)
{
    CPlayerHP* pInstance = new CPlayerHP(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CPlayerHP");
        Safe_Release(pInstance);
    }

    CUI_Mgr::Get_Instance()->Get_vecPlayerHPlist()->push_back(pInstance);

    return pInstance;
}

void CPlayerHP::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
}
