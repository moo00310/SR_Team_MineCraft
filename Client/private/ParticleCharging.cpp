#include "ParticleCharging.h"

CParticleCharging::CParticleCharging(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleCharging::CParticleCharging(const CParticleCharging& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleCharging::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleCharging::Initialize(void* pArg)
{
	iParticleCount = 80;
	IsTextureAnimation = true;
	fAnimationFrame = 0.1f;
	iCurrentTextureIndex = 0;
	iTextureIndex = 8;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 80;
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

CGameObject* CParticleCharging::Clone(void* pArg)
{
	CParticleCharging* pInstance = new CParticleCharging(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleCharging");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleCharging* CParticleCharging::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleCharging* pInstance = new CParticleCharging(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleCharging");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleCharging::Free()
{
	__super::Free();
}

HRESULT CParticleCharging::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXUTRE_GLITTER,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleCharging::OnSetAddParticle()
{
	ParticleAttribute att;	
	att.vPosition = { 0.f, 0.f, 0.f };
	att.vVelocity = {0.f, 0.f, 0.f};

	return att;
}

void CParticleCharging::OnBoundingExit(ParticleAttribute& particle)
{
}
