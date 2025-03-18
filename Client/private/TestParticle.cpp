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
	iParticleCount = 2000;
	dwVpSBatchSize = 500;
	dwPointSize = GetScale(0.2f);
	dwPointScaleA = GetScale(0.f);
	dwPointScaleB = GetScale(0.f);
	dwPointScaleC = GetScale(1.f);			

	SetParticleAttribute();

	// SetParticleAttribute 하고 반드시 
	// 부모 Initialize 호출시켜서 버텍스 초기화 시킬 것.
	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}		

	return S_OK;
}

void CTestParticle::Priority_Update(_float fTimeDelta)
{
}

void CTestParticle::Update(_float fTimeDelta)
{	
	__super::Update(fTimeDelta);
}

void CTestParticle::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CTestParticle::Render()
{
	m_pParticleTexture->Bind_Resource(0);

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
}

HRESULT CTestParticle::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Diamond_ore"),
		TEXT("Com_Component_Diamond_Ore2"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CTestParticle::AddParticle()
{
	ParticleAttribute att;
	att.vPosition = { 0.f, 0.f, 0.f };
	att.vColor = { 1.f, 1.f, 1.f, 1.f };
	att.vVelocity = { GetRandomFloat(-2.f, 2.f), GetRandomFloat(1.f, 2.f), 0.f};
	att.fCurrentTime = 0.f;
	att.fEndTime = 3.f;
	att.IsTime = true;

	return att;
}

HRESULT CTestParticle::PrevRender()
{
	if (FAILED(__super::PrevRender()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTestParticle::EndRender()
{
	if (FAILED(__super::PrevRender()))
	{
		return E_FAIL;
	}

	return S_OK;
}
