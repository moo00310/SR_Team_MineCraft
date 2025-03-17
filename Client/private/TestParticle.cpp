#include "TestParticle.h"

CTestParticle::CTestParticle(LPDIRECT3DDEVICE9 pGraphic_Device) : CParticleSystem(pGraphic_Device)
{
}

CTestParticle::CTestParticle(const CTestParticle& Prototype) : CParticleSystem(Prototype)
{
}

HRESULT CTestParticle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTestParticle::Initialize(void* pArg)
{	
	m_iParticleCount = 2;
	/*vbSize = 2048;
	vbOffset = 0;
	vbBatchSize = 512;*/

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	return S_OK;
}

void CTestParticle::Priority_Update(_float fTimeDelta)
{
}

void CTestParticle::Update(_float fTimeDelta)
{
	int a = 10;
}

void CTestParticle::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CTestParticle::Render()
{
	m_pTextureCom->Bind_Resource(0);
	__super::Render();

	return S_OK;
}

CGameObject* CTestParticle::Clone(void* pArg)
{
	CTestParticle* pInstance = new CTestParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTestParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTestParticle* CTestParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTestParticle* pInstance = new CTestParticle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTestParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTestParticle::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

HRESULT CTestParticle::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Diamond_ore"),
		TEXT("Com_Component_Diamond_Ore2"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTestParticle::PrevRender()
{
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, 20);

	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, 10);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, 20);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, 30);

	return S_OK;
}

HRESULT CTestParticle::EndRender()
{
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);

	return S_OK;
}
