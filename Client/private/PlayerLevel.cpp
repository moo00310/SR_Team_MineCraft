#include "PlayerLevel.h"
#include "UI_Mgr.h"

CPlayerLevel::CPlayerLevel(LPDIRECT3DDEVICE9 pGraphic_Device)
    :CUIObject{ pGraphic_Device }
{
}

CPlayerLevel::CPlayerLevel(CPlayerLevel& Prototype)
    :CUIObject(Prototype)
{
}

HRESULT CPlayerLevel::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerLevel::Initialize(void* pArg)
{
    m_iLevelCount = (int*)pArg;
    m_iLevelIndex = *m_iLevelCount;

    if (m_iLevelIndex == 0)
    {
        Desc.fSizeX = 20.f;
        Desc.fSizeY = 25.f;
        Desc.fX = 655.f;
        Desc.fY = 580.f;
        m_iTextureNum = 9;
        m_bRenderOn = true;
    }    

    else if (m_iLevelIndex == 1)
    {
        Desc.fSizeX = 20.f;
        Desc.fSizeY = 25.f;
        Desc.fX = 625.f;
        Desc.fY = 580.f;
        m_iTextureNum = 9;
        m_bRenderOn = true;
    }

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    return S_OK;
}

void CPlayerLevel::Priority_Update(_float fTimeDelta)
{
}

void CPlayerLevel::Update(_float fTimeDelta)
{
}

void CPlayerLevel::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
        return;
}

HRESULT CPlayerLevel::Render()
{
    if (m_bRenderOn)
    {
        if (FAILED(m_pTextureCom->Bind_Resource(m_iTextureNum)))
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

		m_pShaderCom->Bind_Texture("g_Texture", m_pTextureCom->Get_Texture(m_iTextureNum));

		m_pShaderCom->Begin(1);

        if (FAILED(m_pVIBufferCom->Render()))
            return E_FAIL;

        m_pShaderCom->End();

        __super::End();
    }

    return S_OK;
}

HRESULT CPlayerLevel::Ready_Components()
{
  
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_PlayerLevel"), TEXT("Com_Texture"),
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

CPlayerLevel* CPlayerLevel::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CPlayerLevel* pInstance = new CPlayerLevel(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPlayerLevel");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayerLevel::Clone(void* pArg)
{
    CPlayerLevel* pInstance = new CPlayerLevel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CPlayerLevel");
        Safe_Release(pInstance);
    }

    CUI_Mgr::Get_Instance()->Get_PlayerLevellist()->push_back(pInstance);

    return pInstance;
}

void CPlayerLevel::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
}
