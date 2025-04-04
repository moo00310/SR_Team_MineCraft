#include "BaseParticleEating.h"

CBaseParticleEating::CBaseParticleEating(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CBaseParticleEating::CBaseParticleEating(const CBaseParticleEating& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CBaseParticleEating::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBaseParticleEating::Initialize(void* pArg)
{
	iParticleCount = 5;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 5;
	dwPointSize = GetScale(0.18f);	// ����Ʈ ��������Ʈ ũ��.
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

HRESULT CBaseParticleEating::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_SAND_MINING,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform))))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CBaseParticleEating::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { 0.f, 0.f, 0.f };	
	att.vVelocity = { GetRandomFloat(-3.f, 3.f), 0.f, 0.f };	

	// �߷� ����.
	att.IsGravity = true;
	att.fGravityJumpPower = 0.8f;
	att.fGravityTime = 0.f;
	att.fGravity = 5.8f;

	return att;
}