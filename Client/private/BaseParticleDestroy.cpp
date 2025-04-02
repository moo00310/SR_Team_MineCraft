#include "BaseParticleDestroy.h"

CBaseParticleDestroy::CBaseParticleDestroy(LPDIRECT3DDEVICE9 pGraphic_Device) : 
	CParticleSystem(pGraphic_Device)
{
}

CBaseParticleDestroy::CBaseParticleDestroy(const CBaseParticleDestroy& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CBaseParticleDestroy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBaseParticleDestroy::Initialize(void* pArg)
{
	iParticleCount = 20;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 20;
	dwPointSize = GetScale(0.18f);	// 포인트 스프라이트 크기.
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

void CBaseParticleDestroy::Free()
{
	__super::Free();
}

ParticleAttribute CBaseParticleDestroy::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { 0.f, 0.f, 0.f };
	//att.vColor = Float3ToHex({ 0.f, 0.f, 0.f });
	att.vVelocity = { GetRandomFloat(-3.f, 3.f), 0.f, GetRandomFloat(-3.f, 3.f) };
	att.IsTime = true;
	att.fCurrentTime = 0.f;
	att.fEndTime = 0.6f;
	att.IsAlive = false;

	// 중력 적용.
	att.IsGravity = true;
	att.fGravityJumpPower = GetRandomFloat(1.8f, 2.f);
	att.fGravityTime = 0.f;
	att.fGravity = 9.8f;

	return att;
}

void CBaseParticleDestroy::OnBoundingExit(ParticleAttribute& particle)
{
}

void CBaseParticleDestroy::OnReplay(ParticleAttribute& particle)
{
	particle.vVelocity = { GetRandomFloat(-3.f, 3.f), 0.f, GetRandomFloat(-3.f, 3.f) };
	particle.fGravityJumpPower = GetRandomFloat(1.8f, 2.f);
}
