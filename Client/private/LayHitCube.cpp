#include "LayHitCube.h"

CLayHitCube::CLayHitCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCube(pGraphic_Device)
{
}

CLayHitCube::CLayHitCube(const CLayHitCube& Prototype)
	: CCube(Prototype)
{
}

HRESULT CLayHitCube::PrevRender()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CLayHitCube::EndRender()
{
	// 알파 텍스쳐 비활성화.
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

HRESULT CLayHitCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLayHitCube::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	SetActive(false);

	return S_OK;
}

void CLayHitCube::Priority_Update(_float fTimeDelta)
{
}

void CLayHitCube::Update(_float fTimeDelta)
{
}

void CLayHitCube::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CLayHitCube::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
	{
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferOnlyCom->Bind_Buffers()))
	{
		return E_FAIL;
	}

	if (FAILED(PrevRender()))
	{
		return E_FAIL;
	}

	if (FAILED(m_pVIBufferOnlyCom->Render()))
	{
		return E_FAIL;
	}

	if (FAILED(EndRender()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLayHitCube::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_YU, PROTOTYPE_COMPONENT_VIBUFFER_ONLY,
		TEXT("m_pVIBufferOnlyCom"), reinterpret_cast<CComponent**>(&m_pVIBufferOnlyCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CLayHitCube* CLayHitCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLayHitCube* pInstance = new CLayHitCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLayHitCube");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject* CLayHitCube::Clone(void* pArg)
{
	CLayHitCube* pInstance = new CLayHitCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CLayHitCube");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CLayHitCube::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferOnlyCom);
}

CTransform* CLayHitCube::GetTransform() const
{
	return m_pTransformCom;
}
