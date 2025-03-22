#include "Clouds.h"

CClouds::CClouds(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CClouds::CClouds(const CClouds& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CClouds::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CClouds::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 100.f, 0.f));
	m_pTransformCom->Scaling(5000.f, 5000.f, 1.f);
	m_pTransformCom->Rotation({ 1.f, 0.f, 0.f }, D3DXToRadian(-90.f));

	return S_OK;
}

void CClouds::Priority_Update(_float fTimeDelta)
{
}

void CClouds::Update(_float fTimeDelta)
{

}


void CClouds::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CClouds::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	SetUp_RenderState();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	Release_RenderState();

	return S_OK;
}

HRESULT CClouds::SetUp_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	return S_OK;
}

HRESULT CClouds::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CClouds::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Clouds"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CClouds* CClouds::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CClouds* pInstance = new CClouds(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CClouds");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CClouds::Clone(void* pArg)
{
	CClouds* pInstance = new CClouds(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CClouds");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClouds::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
