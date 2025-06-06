#include "ParticleFurnace.h"

CParticleFurnace::CParticleFurnace(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleFurnace::CParticleFurnace(const CParticleFurnace& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleFurnace::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleFurnace::Initialize(void* pArg)
{
	iParticleCount = 5;		
	IsBounding = true;
	IsTimer = true;
	fEndTimer = 150.f;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 5;
	dwPointSize = GetScale(0.15f);	// 포인트 스프라이트 크기.
	dwPointScaleA = GetScale(0.f);	// 포인트 스프라이트 거리별 크기.
	dwPointScaleB = GetScale(0.f);
	dwPointScaleC = GetScale(1.f);

	ParticleBoundingBox box;
	box.vMinPosition = { -0.3f, 0.f, 0.f };		// 최소 범위.
	box.vMaxPosition = { 0.3f, 0.2f, 0.f };		// 최대 범위.

	// 파티클 경계선 셋팅 작업.
	SetParticleBoundingBox(box);

	// 부모 Initialize 호출시켜서 버텍스 초기화 시키고 이거 호출 시킬 것.
	if (FAILED(Create_VertexBuffer()))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}	
}

CGameObject* CParticleFurnace::Clone(void* pArg)
{
	CParticleFurnace* pInstance = new CParticleFurnace(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleFurnace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleFurnace* CParticleFurnace::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleFurnace* pInstance = new CParticleFurnace(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleFurnace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CParticleFurnace::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_FLAME,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleFurnace::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { GetRandomFloat(-0.3f, 0.3f), GetRandomFloat(0.f, 0.1f), 0.f};
	att.vColor = Float3ToHex({ 1.f, 1.f, 1.f });
	att.vVelocity = { 0.f, 0.1f, 0.f };	

	return att;
}

void CParticleFurnace::OnBoundingExit(ParticleAttribute& particle)
{
	particle.vPosition = { GetRandomFloat(-0.3f, 0.3f), GetRandomFloat(0.f, 0.1f), 0.f };
}
