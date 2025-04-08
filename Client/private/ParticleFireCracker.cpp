#include "ParticleFireCracker.h"

CParticleFireCracker::CParticleFireCracker(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleFireCracker::CParticleFireCracker(const CParticleFireCracker& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleFireCracker::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleFireCracker::Initialize(void* pArg)
{
	iParticleCount = 80;
	IsTimer = true;	
	fEndTimer = 1.f;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 80;
	dwPointSize = GetScale(0.2f);	// ����Ʈ ��������Ʈ ũ��.
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

CGameObject* CParticleFireCracker::Clone(void* pArg)
{
	CParticleFireCracker* pInstance = new CParticleFireCracker(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleFireCracker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleFireCracker* CParticleFireCracker::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleFireCracker* pInstance = new CParticleFireCracker(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleFireCracker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleFireCracker::Free()
{
	__super::Free();
}

HRESULT CParticleFireCracker::Ready_Components()
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

ParticleAttribute CParticleFireCracker::OnSetAddParticle()
{	
	// �� ���⿡ ���� ���� ����.
	_float3 velocity = { GetRandomFloat(-1.f, 1.f), GetRandomFloat(-1.f, 1.f), GetRandomFloat(-1.f, 1.f) };

	// ����ȭ.
	D3DXVec3Normalize(&velocity, &velocity);

	// �ӵ� ��.
	velocity *= 4.4f;

	ParticleAttribute att;
	att.vPosition = {0.f, 0.f, 0.f};	
	att.vColor = Float3ToHex({ GetRandomFloat(0.f, 1.f), GetRandomFloat(0.f, 1.f), GetRandomFloat(0.f, 1.f) });
	att.vVelocity = velocity;	

	// �߷� ����.
	att.IsGravity = true;
	att.fGravity = 0.4f;
	att.fGravityJumpPower = 0.f;
	att.fGravityTime = 0.f;	

	return att;
}

void CParticleFireCracker::OnBoundingExit(ParticleAttribute& particle)
{
}
