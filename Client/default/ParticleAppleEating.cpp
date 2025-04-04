#include "CParticleAppleEating.h"

CParticleAppleEating::CParticleAppleEating(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CBaseParticleEating(pGraphic_Device)
{
}

CParticleAppleEating::CParticleAppleEating(const CParticleAppleEating& Prototype) :
	CBaseParticleEating(Prototype)
{
}

CGameObject* CParticleAppleEating::Clone(void* pArg)
{
	CParticleAppleEating* pInstance = new CParticleAppleEating(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleAppleEating");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleAppleEating* CParticleAppleEating::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleAppleEating* pInstance = new CParticleAppleEating(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleDash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleAppleEating::Free()
{
	__super::Free();
}

ParticleAttribute CParticleAppleEating::OnSetAddParticle()
{
	ParticleAttribute att = __super::OnSetAddParticle();

	att.vColor = Float3ToHex({1.f, 0.f, 0.f});

	return att;
}

void CParticleAppleEating::OnBoundingExit(ParticleAttribute& particle)
{
}
