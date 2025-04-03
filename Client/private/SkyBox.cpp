#include "SkyBox.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CSkyBox::CSkyBox(const CSkyBox& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CSkyBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkyBox::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSkyBox::Priority_Update(_float fTimeDelta)
{

}

void CSkyBox::Update(_float fTimeDelta)
{

}

void CSkyBox::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);

	_float4x4		ViewMatrix{};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *reinterpret_cast<_float3*>(&ViewMatrix.m[3][0]));
}

HRESULT CSkyBox::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	m_pTransformCom->Bind_Resource(m_pShaderCom);
	m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);
	m_pShaderCom->SetFloat("g_Bright", g_fBright + 0.2f);
	m_pShaderCom->Begin(0);

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CSkyBox::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_GrassDirt"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_CubeShader"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Cube"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSkyBox* pInstance = new CSkyBox(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkyBox::Clone(void* pArg)
{
	CSkyBox* pInstance = new CSkyBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkyBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
