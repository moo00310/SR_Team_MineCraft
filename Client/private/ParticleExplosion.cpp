#include "ParticleExplosion.h"

CParticleExplosion::CParticleExplosion(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParticleSystem(pGraphic_Device)
{
}

CParticleExplosion::CParticleExplosion(const CParticleExplosion& Prototype)
	: CParticleSystem(Prototype)
{
}

HRESULT CParticleExplosion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleExplosion::Initialize(void* pArg)
{
	iParticleCount = 12;
	iTextureIndex = 16;
	fAnimationFrame = 0.05f;
	IsTextureAnimation = true;
	IsTimer = true;
	fEndTimer = 0.8f;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 12;
	dwPointSize = GetScale(1.f);	// 포인트 스프라이트 크기.
	dwPointScaleA = GetScale(0.f);	// 포인트 스프라이트 거리별 크기.
	dwPointScaleB = GetScale(0.f);
	dwPointScaleC = GetScale(3.f);

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

CGameObject* CParticleExplosion::Clone(void* pArg)
{
	CParticleExplosion* pInstance = new CParticleExplosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleExplosion* CParticleExplosion::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleExplosion* pInstance = new CParticleExplosion(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleExplosion::Free()
{
	__super::Free();
}

HRESULT CParticleExplosion::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_EXPLOSION,
		TEXT("Com_Component_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Component_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleExplosion::OnSetAddParticle()
{	
	ParticleAttribute att;
	att.vPosition = { GetRandomFloat(-2.f, 2.f), GetRandomFloat(-2.f, 2.f), GetRandomFloat(-2.f, 2.f)};
	//att.vColor = Float3ToHex({ 0.f, 1.f, 0.f });
	att.vVelocity = { 0.f, 0.f, 0.f };
	//att.IsTime = true;
	att.fCurrentTime = 0.f;
	//att.fEndTime = 7.4f;

	// 첫 생성 시 파티클이 나오지 않게 비활성화.
	att.IsAlive = false;

	return att;
}

void CParticleExplosion::OnBoundingExit(ParticleAttribute& particle)
{
}
