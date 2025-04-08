#include "ParticleSonicBoom.h"

CParticleSonicBoom::CParticleSonicBoom(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleSonicBoom::CParticleSonicBoom(const CParticleSonicBoom& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleSonicBoom::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleSonicBoom::Initialize(void* pArg)
{
	iParticleCount = 10;
	IsTimer = true;	
	fEndTimer = 1.4f;
	IsTextureAnimation = true;
	fAnimationFrame = 0.1f;
	iTextureIndex = 15;	

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 10;
	dwPointSize = GetScale(6.5f);	// 포인트 스프라이트 크기.
	dwPointScaleA = GetScale(0.f);	// 포인트 스프라이트 거리별 크기.
	dwPointScaleB = GetScale(0.f);
	dwPointScaleC = GetScale(1.f);

	// 부모 Initialize 호출시켜서 버텍스 초기화 시키고 이거 호출 시킬 것.
	if (FAILED(Create_VertexBuffer()))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}

	return S_OK;
}

CGameObject* CParticleSonicBoom::Clone(void* pArg)
{
	CParticleSonicBoom* pInstance = new CParticleSonicBoom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleSonicBoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleSonicBoom* CParticleSonicBoom::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleSonicBoom* pInstance = new CParticleSonicBoom(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleSonicBoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleSonicBoom::OnPushPool()
{
	__super::OnPushPool();

	m_particleIndex = 0;
}

HRESULT CParticleSonicBoom::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_SONIC_BOOM,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleSonicBoom::OnSetAddParticle()
{	
	ParticleAttribute att;
	att.vPosition = { 0.f, 0.f, (_float)m_particleIndex };
	//att.vColor = Float3ToHex({GetRandomFloat(0.1f, 0.5f), 0.f, 0.f});
	att.vVelocity = {0.f, 0.f, 0.f};

	m_particleIndex++;

	return att;
}

void CParticleSonicBoom::OnBoundingExit(ParticleAttribute& particle)
{
}
