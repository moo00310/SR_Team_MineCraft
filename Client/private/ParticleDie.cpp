#include "ParticleDie.h"

CParticleDie::CParticleDie(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleDie::CParticleDie(const CParticleDie& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleDie::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleDie::Initialize(void* pArg)
{
	iParticleCount = 6;
	IsTimer = true;
	fEndTimer = 2.f;
	IsParticleTexture = true;

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

CGameObject* CParticleDie::Clone(void* pArg)
{
	CParticleDie* pInstance = new CParticleDie(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleDie");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleDie* CParticleDie::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleDie* pInstance = new CParticleDie(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleDie");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CParticleDie::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_DIE,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	/*if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_SHADER_PARTICLE,
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShader))))
		return E_FAIL;*/

	return S_OK;
}

ParticleAttribute CParticleDie::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { GetRandomFloat(-0.5f, 0.5f), GetRandomFloat(-0.5f, 0.5f), GetRandomFloat(-0.5f, 0.5f) };
	att.vColor = Float3ToHex({ 1.f, 1.f, 1.f });
	att.vVelocity = { GetRandomFloat(-0.2f, 0.2f), 0.2f, 0.f };
	att.iTextureIndex = (int)GetRandomFloat(0.f, 3.f);

	return att;
}

void CParticleDie::OnBoundingExit(ParticleAttribute& particle)
{
}
