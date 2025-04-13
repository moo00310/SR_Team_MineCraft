#include "BurnResultUi.h"

CBurnResultUi::CBurnResultUi(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CBurnResultUi::CBurnResultUi(CBurnResultUi& Prototype)
	: CUIObject( Prototype )
{
}

HRESULT CBurnResultUi::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBurnResultUi::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};

	Desc.fSizeX = 75.f;
	Desc.fSizeY = 50.f;
	Desc.fX = 650.f;
	Desc.fY = 175.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	return S_OK;
}

void CBurnResultUi::Priority_Update(_float fTimeDelta)
{
}

void CBurnResultUi::Update(_float fTimeDelta)
{
}

void CBurnResultUi::Late_Update(_float fTimeDelta)
{
}

HRESULT CBurnResultUi::Render(_float _ironTime)
{
	if (g_bFurnaceUiOpen)
	{
		if (FAILED(m_pTextureCom->Bind_Resource(0)))
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
		m_pShaderCom->SetFloat("g_burnResult", _ironTime);

		m_pShaderCom->Begin(6);

		/* 정점을 그린다. */
		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		m_pShaderCom->End();
		__super::End();

	}
	return S_OK;
}

HRESULT CBurnResultUi::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_BurnResult"), TEXT("Com_Texture"),
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

CBurnResultUi* CBurnResultUi::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBurnResultUi* pInstance = new CBurnResultUi(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBurnResultUi");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBurnResultUi::Clone(void* pArg)
{
	CBurnResultUi* pInstance = new CBurnResultUi(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBurnResultUi");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBurnResultUi::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);

}
