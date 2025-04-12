#include "ParticleFurnaceCloud.h"

CParticleFurnaceCloud::CParticleFurnaceCloud(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleFurnaceCloud::CParticleFurnaceCloud(const CParticleFurnaceCloud& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleFurnaceCloud::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleFurnaceCloud::Initialize(void* pArg)
{
	iParticleCount = 3;
	IsBounding = true;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 3;
	dwPointSize = GetScale(0.2f);	// 포인트 스프라이트 크기.
	dwPointScaleA = GetScale(0.f);	// 포인트 스프라이트 거리별 크기.
	dwPointScaleB = GetScale(0.f);
	dwPointScaleC = GetScale(1.f);

	ParticleBoundingBox box;
	box.vMinPosition = { -0.3f, 0.f, 0.f };		// 최소 범위.
	box.vMaxPosition = { 0.3f, 1.2f, 0.f };		// 최대 범위.

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

CGameObject* CParticleFurnaceCloud::Clone(void* pArg)
{
	CParticleFurnaceCloud* pInstance = new CParticleFurnaceCloud(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleFurnaceCloud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleFurnaceCloud* CParticleFurnaceCloud::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleFurnaceCloud* pInstance = new CParticleFurnaceCloud(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleFurnaceCloud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CParticleFurnaceCloud::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_DASH,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleFurnaceCloud::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { GetRandomFloat(-0.3f, 0.3f), GetRandomFloat(0.f, 0.1f), 0.f };
	att.vColor = Float3ToHex({ 1.f, 1.f, 1.f });
	att.vVelocity = { 0.f, GetRandomFloat(0.2f, 0.3f), 0.f };

	return att;
}

void CParticleFurnaceCloud::OnBoundingExit(ParticleAttribute& particle)
{
	particle.vPosition = { GetRandomFloat(-0.3f, 0.3f), GetRandomFloat(0.f, 0.1f), 0.f };
	particle.vVelocity = { 0.f, GetRandomFloat(0.2f, 0.3f), 0.f };
}
