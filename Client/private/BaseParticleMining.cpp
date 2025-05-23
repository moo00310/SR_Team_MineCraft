#include "BaseParticleMining.h"

// 캘 때 파티클.
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
	m_Sun = (CSun*)m_pGameInstance->Get_Object(LEVEL_YU,
		TEXT("Layer_Sun"),
		0);

	iParticleCount = 1;
	IsTimer = true;
	fEndTimer = 0.6f;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 1;
	dwPointSize = GetScale(0.1f);	// 포인트 스프라이트 크기.
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

void CBaseParticleMining::Free()
{
	__super::Free();
}

void CBaseParticleMining::OnPushPool()
{
	__super::OnPushPool();
	IsTimer = true;
	fEndTimer = 0.6f;
}

ParticleAttribute CBaseParticleMining::OnSetAddParticle()
{
	float bight = m_Sun->GetBight();

	ParticleAttribute att;	
	att.vPosition = { GetRandomFloat(-0.2f, 0.2f), GetRandomFloat(-0.2f, 0.2f), 0.f };	
	att.vColor = Float3ToHex({ bight, bight, bight });
	att.vVelocity = { GetRandomFloat(-2.f, 2.f), 0.f, GetRandomFloat(-2.f, 2.f) };	
	att.IsAlive = false;

	// 중력 적용.
	att.IsGravity = true;
	att.fGravityJumpPower = GetRandomFloat(0.5f, 1.f);
	att.fGravityTime = 0.f;
	att.fGravity = 6.8f;

	return att;
}

void CBaseParticleMining::OnBoundingExit(ParticleAttribute& particle)
{
}

void CBaseParticleMining::OnReplay(ParticleAttribute& particle)
{
	particle.vVelocity = { GetRandomFloat(-2.f, 2.f), 0.f, GetRandomFloat(-2.f, 2.f) };
	particle.fGravityJumpPower = GetRandomFloat(0.5f, 1.f);
	particle.fGravityTime = 0.f;
}
