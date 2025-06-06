#include "ParticleSwordFlame.h"

CParticleSwordFlame::CParticleSwordFlame(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleSwordFlame::CParticleSwordFlame(const CParticleSwordFlame& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleSwordFlame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleSwordFlame::Initialize(void* pArg)
{
	iParticleCount = 20;
	IsTimer = false;
	IsBounding = true;	

	ParticleBoundingBox box;
	box.vMinPosition = {0.f, -0.1f, -0.5f};
	box.vMaxPosition = { 0.5f, 0.3f, -0.1f };
	SetParticleBoundingBox(box);

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 20;
	dwPointSize = GetScale(0.1f);	// 포인트 스프라이트 크기.
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

void CParticleSwordFlame::Late_Update(_float fTimeDelta)
{
	// 불검 파티클은 오른손이 깊이 값이 꺼져서 순서를 늦게 출력하기 위해 블렌드로 줌.
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CParticleSwordFlame::Render()
{
	// 불검 파티클은 오른손이 깊이 값이 꺼져서 같이 꺼줌.
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);

	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, true);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	return S_OK;
}

CGameObject* CParticleSwordFlame::Clone(void* pArg)
{
	CParticleSwordFlame* pInstance = new CParticleSwordFlame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleSwordFlame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleSwordFlame* CParticleSwordFlame::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleSwordFlame* pInstance = new CParticleSwordFlame(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleSwordFlame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleSwordFlame::Free()
{
	__super::Free();
}

HRESULT CParticleSwordFlame::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_FLAME,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;	

	return S_OK;
}

ParticleAttribute CParticleSwordFlame::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { GetRandomFloat(0.25f, 0.35f), GetRandomFloat(0.f, 0.2f), GetRandomFloat(-0.4f, -0.2f) };
	att.vColor = Float3ToHex({ 0.6f, 0.6f, 0.6f });
	att.vVelocity = { GetRandomFloat(-0.2f, 0.2f), 0.4f, 0.1f };
	att.IsTime = false;
	att.fCurrentTime = 0.f;
	//att.fEndTime = 0.4f;	
	att.IsAlive = false;

 	return att;
}

void CParticleSwordFlame::OnBoundingExit(ParticleAttribute& particle)
{
	particle.vPosition = { GetRandomFloat(0.25f, 0.35f), GetRandomFloat(0.f, 0.2f), GetRandomFloat(-0.4f, -0.2f) };
}
