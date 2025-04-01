#include "DestroyCube.h"

CDestroyCube::CDestroyCube(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CCube(pGraphic_Device)
{
	Safe_AddRef(m_pVIBufferOnlyCom);
}

CDestroyCube::CDestroyCube(const CDestroyCube& Prototype) :
	CCube(Prototype)
{
}

HRESULT CDestroyCube::PrevRender()
{
	// 알파텍스쳐 활성화.
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CDestroyCube::EndRender()
{
	// 알파 텍스쳐 비활성화.
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CDestroyCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDestroyCube::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	// 테스트용 블럭 확인 위치.
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, {0.f, 2.f, 0.f});

	return S_OK;
}

void CDestroyCube::Priority_Update(_float fTimeDelta)
{
}

void CDestroyCube::Update(_float fTimeDelta)
{
}

void CDestroyCube::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CDestroyCube::Render()
{	
	if (FAILED(m_pTextureCom->Bind_Resource(4)))
	{
		return E_FAIL;
	}

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

HRESULT CDestroyCube::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_YU, PROTOTYPE_COMPONENT_VIBUFFER_ONLY,
	    TEXT("m_pVIBufferOnlyCom"), reinterpret_cast<CComponent**>(&m_pVIBufferOnlyCom))))
	    return E_FAIL;

	/* For.Com_Texture */	
	if (FAILED(__super::Add_Component(LEVEL_YU, PROTOTYPE_COMPONENT_TEXTURE_DESTROY,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CDestroyCube* CDestroyCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDestroyCube* pInstance = new CDestroyCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDestroyCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDestroyCube::Clone(void* pArg)
{
	CDestroyCube* pInstance = new CDestroyCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CDestroyCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDestroyCube::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferOnlyCom);
}
