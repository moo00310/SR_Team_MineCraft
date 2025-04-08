#include "ParticleCrackerLoad.h"

CParticleCrackerLoad::CParticleCrackerLoad(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleCrackerLoad::CParticleCrackerLoad(const CParticleCrackerLoad& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleCrackerLoad::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleCrackerLoad::Initialize(void* pArg)
{
	iParticleCount = 1;
	IsTimer = true;
	fEndTimer = 0.5f;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 1;
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

CGameObject* CParticleCrackerLoad::Clone(void* pArg)
{
	CParticleCrackerLoad* pInstance = new CParticleCrackerLoad(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleCrackerLoad");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleCrackerLoad* CParticleCrackerLoad::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleCrackerLoad* pInstance = new CParticleCrackerLoad(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleCrackerLoad");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleCrackerLoad::Free()
{
	__super::Free();
}

HRESULT CParticleCrackerLoad::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_GLINT,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleCrackerLoad::OnSetAddParticle()
{	
	ParticleAttribute att;
	att.vPosition = { 0.f, 0.f, 0.f };
	att.IsFrameColor = true;
	att.vVelocity = {GetRandomFloat(-1.f, 1.f), -0.5f, 0.f};	

	return att;
}

void CParticleCrackerLoad::OnBoundingExit(ParticleAttribute& particle)
{
}
