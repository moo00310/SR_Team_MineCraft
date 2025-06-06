#include "PlayerHunger.h"
#include "UI_Mgr.h"

CPlayerHunger::CPlayerHunger(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CPlayerHunger::CPlayerHunger(CPlayerHunger& Prototype)
	: CUIObject( Prototype )
{
}

HRESULT CPlayerHunger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerHunger::Initialize(void* pArg)
{
    m_iHungerCount = (int*)pArg;
    m_iHungerIndex = *m_iHungerCount;

    m_iTextureNum = 1;
    Desc.fSizeX = 25.f;
    Desc.fSizeY = 25.f;
    Desc.fX = 680.f + (m_iHungerIndex * 30.f);
    Desc.fY = 570.f;
    m_bFlicker = false;

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CPlayerHunger::Priority_Update(_float fTimeDelta)
{
}

void CPlayerHunger::Update(_float fTimeDelta)
{
}

void CPlayerHunger::Late_Update(_float fTimeDelta)
{
    m_fTiem += fTimeDelta;

    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CPlayerHunger::Render()
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

	if(m_bFlicker)
	{
        if (m_fTiem < 1.f)
        {
            float timeValue = GetTickCount64() * 0.001f;
            m_pShaderCom->SetFloat("g_Time", timeValue);

            m_pShaderCom->Begin(2);

        }
        else
        {
            m_fTiem = 0.f;
            m_bFlicker = false;
        }
	}

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

    m_pShaderCom->End();
	__super::End();
    Reset_RenderState();

	return S_OK;
}

HRESULT CPlayerHunger::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_PlayerHunger"), TEXT("Com_Texture"),
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

HRESULT CPlayerHunger::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 160);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    
    return S_OK;
}

HRESULT CPlayerHunger::Reset_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

CPlayerHunger* CPlayerHunger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayerHunger* pInstance = new CPlayerHunger(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPlayerHunger");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CPlayerHunger::Clone(void* pArg)
{
    CPlayerHunger* pInstance = new CPlayerHunger(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CPlayerHunger");
        Safe_Release(pInstance);
    }

    CUI_Mgr::Get_Instance()->Get_PlayerHungerlist()->push_back(pInstance);

    return pInstance;
}

void CPlayerHunger::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
}
