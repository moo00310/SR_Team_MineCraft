#include "ParticleDig.h"

CParticleDig::CParticleDig(LPDIRECT3DDEVICE9 pGraphic_Device) : CParticleSystem(pGraphic_Device)
{
}

CParticleDig::CParticleDig(const CParticleDig& Prototype) : CParticleSystem(Prototype)
{
}

HRESULT CParticleDig::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleDig::Initialize(void* pArg)
{
	iParticleCount = 6;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 6;
	dwPointSize = GetScale(1.f);	// 포인트 스프라이트 크기.
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

CGameObject* CParticleDig::Clone(void* pArg)
{
	CParticleDig* pInstance = new CParticleDig(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleDig");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleDig* CParticleDig::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleDig* pInstance = new CParticleDig(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleDig");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleDig::Free()
{
	__super::Free();
}

HRESULT CParticleDig::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Dig"),
		TEXT("Com_Component_Dig"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleDig::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { 0.f, 0.f, 0.f };
	att.vColor = { GetRandomFloat(0.1f, 0.2f), 0.f, 0.f, 1.f };
	att.vVelocity = { GetRandomFloat(-3.f, 3.f), 0.f, 0.f };
	att.IsTime = true;
	att.fCurrentTime = 0.f;
	att.fEndTime = 0.6f;	
	att.IsAlive = false;

	// 중력 적용.
	att.IsGravity = true;
	att.fGravityJumpPower = 1.8f;
	att.fGravityTime = 0.f;
	att.fGravity = 8.8f;

	return att;
}

void CParticleDig::OnBoundingExit(ParticleAttribute& particle)
{
}

void CParticleDig::OnReplay(ParticleAttribute& particle)
{
	particle.vVelocity = { GetRandomFloat(-3.f, 3.f), 0.f, 0.f };
}
