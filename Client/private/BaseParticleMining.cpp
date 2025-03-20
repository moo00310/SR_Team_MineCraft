#include "BaseParticleMining.h"

// 나무 캘 때 파티클.
CBaseParticleMining::CBaseParticleMining(LPDIRECT3DDEVICE9 pGraphic_Device) : CParticleSystem(pGraphic_Device)
{
}

CBaseParticleMining::CBaseParticleMining(const CBaseParticleMining& Prototype) : CParticleSystem(Prototype)
{
}

HRESULT CBaseParticleMining::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBaseParticleMining::Initialize(void* pArg)
{
	iParticleCount = 10;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 10;
	dwPointSize = GetScale(1.f);	// 포인트 스프라이트 크기.
	dwPointScaleA = GetScale(0.f);	// 포인트 스프라이트 거리별 크기.
	dwPointScaleB = GetScale(0.f);
	dwPointScaleC = GetScale(2.f);

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

void CBaseParticleMining::Free()
{
	__super::Free();
}

ParticleAttribute CBaseParticleMining::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { 0.f, 0.f, 0.f };
	att.vColor = { 0.1f, 0.f, 0.f, 1.f };
	att.vVelocity = { GetRandomFloat(-3.f, 3.f), 0.f, 0.f };
	att.IsTime = true;
	att.fCurrentTime = 0.f;
	att.fEndTime = 0.6f;	
	att.IsAlive = false;

	// 중력 적용.
	att.IsGravity = true;
	att.fGravityJumpPower = GetRandomFloat(1.8f, 2.f);
	att.fGravityTime = 0.f;
	att.fGravity = 8.8f;

	return att;
}

void CBaseParticleMining::OnBoundingExit(ParticleAttribute& particle)
{
}

void CBaseParticleMining::OnReplay(ParticleAttribute& particle)
{
	particle.vVelocity = { GetRandomFloat(-3.f, 3.f), 0.f, 0.f };
	particle.fGravityJumpPower = GetRandomFloat(1.8f, 2.f);
}
