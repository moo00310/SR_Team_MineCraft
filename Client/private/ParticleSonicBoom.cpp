#include "ParticleSonicBoom.h"

CParticleSonicBoom::CParticleSonicBoom(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleSonicBoom::CParticleSonicBoom(const CParticleSonicBoom& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleSonicBoom::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleSonicBoom::Initialize(void* pArg)
{
	iParticleCount = 6;
	IsTimer = true;
	fEndTimer = 1.4f;
	IsTextureAnimation = true;
	fAnimationFrame = 0.1f;
	iTextureIndex = 15;
	iCurrentTextureIndex = 0;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 6;
	dwPointSize = GetScale(0.5f);	// ����Ʈ ��������Ʈ ũ��.
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

CGameObject* CParticleSonicBoom::Clone(void* pArg)
{
	CParticleSonicBoom* pInstance = new CParticleSonicBoom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleSonicBoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleSonicBoom* CParticleSonicBoom::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleSonicBoom* pInstance = new CParticleSonicBoom(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleSonicBoom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CParticleSonicBoom::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_SONIC_BOOM,
		TEXT("Com_Component_Dash"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Component_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleSonicBoom::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { 0.f, 0.f, 0.f };
	//att.vColor = Float3ToHex({GetRandomFloat(0.1f, 0.5f), 0.f, 0.f});
	//att.vVelocity = { GetRandomFloat(-1.f, 1.f), 0.f, GetRandomFloat(-3.f, -1.f) };	

	// ù ���� �� ��ƼŬ�� ������ �ʰ� ��Ȱ��ȭ.
	att.IsAlive = false;

	return att;
}

void CParticleSonicBoom::OnBoundingExit(ParticleAttribute& particle)
{
}
