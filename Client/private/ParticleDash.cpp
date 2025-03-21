#include "ParticleDash.h"

// 달리기 파티클.
CParticleDash::CParticleDash(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleDash::CParticleDash(const CParticleDash& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleDash::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleDash::Initialize(void* pArg)
{
	iParticleCount = 6;	

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 6;
	dwPointSize = GetScale(0.2f);	// 포인트 스프라이트 크기.
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

CGameObject* CParticleDash::Clone(void* pArg)
{
	CParticleDash* pInstance = new CParticleDash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleDash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleDash* CParticleDash::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleDash* pInstance = new CParticleDash(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleDash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleDash::Free()
{
	__super::Free();
}

HRESULT CParticleDash::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Dash"),
		TEXT("Com_Component_Dash"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleDash::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = {0.f, 0.f, 0.f};	
	att.vColor = {GetRandomFloat(0.1f, 0.5f), 0.f, 0.f, 1.f };
	att.vVelocity = {GetRandomFloat(-1.f, 1.f), 0.f, GetRandomFloat(-3.f, -1.f) };
	att.IsTime = true;
	att.fCurrentTime = 0.f;
	att.fEndTime = 0.4f;	

	// 첫 생성 시 파티클이 나오지 않게 비활성화.
	att.IsAlive = false;

	return att;
}

void CParticleDash::OnBoundingExit(ParticleAttribute& particle)
{
}