#include "BaseParticleMining.h"

// ���� Ķ �� ��ƼŬ.
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
	iParticleCount = 1;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 1;
	dwPointSize = GetScale(0.1f);	// ����Ʈ ��������Ʈ ũ��.
	dwPointScaleA = GetScale(0.f);	// ����Ʈ ��������Ʈ �Ÿ��� ũ��.
	dwPointScaleB = GetScale(0.f);
	dwPointScaleC = GetScale(1.f);

	// �θ� Initialize ȣ����Ѽ� ���ؽ� �ʱ�ȭ ��Ű�� �̰� ȣ�� ��ų ��.
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
	//att.vColor = Float3ToHex({ 0.1f, 0.f, 0.f });
	att.vVelocity = { GetRandomFloat(-3.f, 3.f), 0.f, 0.f };
	att.IsTime = true;
	att.fCurrentTime = 0.f;
	att.fEndTime = 0.6f;	
	att.IsAlive = false;

	// �߷� ����.
	att.IsGravity = true;
	att.fGravityJumpPower = GetRandomFloat(0.f, 0.5f);
	att.fGravityTime = 0.f;
	att.fGravity = 14.8f;

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
