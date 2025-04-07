#include "ParticleSwordAura.h"

CParticleSwordAura::CParticleSwordAura(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleSwordAura::CParticleSwordAura(const CParticleSwordAura& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleSwordAura::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleSwordAura::Initialize(void* pArg)
{
	iParticleCount = 1;
	IsTimer = true;		
	fEndTimer = 0.5f;

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

CGameObject* CParticleSwordAura::Clone(void* pArg)
{
	CParticleSwordAura* pInstance = new CParticleSwordAura(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleSwordAura");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleSwordAura* CParticleSwordAura::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleSwordAura* pInstance = new CParticleSwordAura(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleSwordAura");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CParticleSwordAura::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTYPE_COMPONENT_TEXTURE_GLOW,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleSwordAura::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { 0.f, 0.f, 0.f };
	att.vColor = Float3ToHex({1.f, 0.f, 0.f});
	att.vVelocity = { GetRandomFloat(-1.f, 1.f), -1.4f, 0.f};
	att.IsTime = false;
	att.fCurrentTime = 0.f;
	att.IsAlive = false;

	return att;
}

void CParticleSwordAura::OnBoundingExit(ParticleAttribute& particle)
{	
}
