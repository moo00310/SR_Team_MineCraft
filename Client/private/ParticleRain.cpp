#include "ParticleRain.h"

CParticleRain::CParticleRain(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CParticleSystem(pGraphic_Device)
{
}

CParticleRain::CParticleRain(const CParticleRain& Prototype) :
	CParticleSystem(Prototype)
{
}

HRESULT CParticleRain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleRain::Initialize(void* pArg)
{
	iParticleCount = 1000;
	IsBounding = true;

	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	dwVpBatchSize = 500;
	dwPointSize = GetScale(2.f);	// ����Ʈ ��������Ʈ ũ��.
	dwPointScaleA = GetScale(0.f);	// ����Ʈ ��������Ʈ �Ÿ��� ũ��.
	dwPointScaleB = GetScale(0.f);
	dwPointScaleC = GetScale(1.f);

	ParticleBoundingBox box;
	box.vMinPosition = { -30.f, -10.f, -30.f };		// �ּ� ����.
	box.vMaxPosition = { 30.f, 30.f, 30.f };		// �ִ� ����.

	// ��ƼŬ ��輱 ���� �۾�.
	SetParticleBoundingBox(box);

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

void CParticleRain::Priority_Update(_float fTimeDelta)
{
}

void CParticleRain::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CParticleRain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CParticleRain::Render()
{
	m_pParticleTexture->Bind_Resource(0);

	if (FAILED(__super::Render()))
	{
		return E_FAIL;
	}

	return S_OK;
}

CGameObject* CParticleRain::Clone(void* pArg)
{
	CParticleRain* pInstance = new CParticleRain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleRain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleRain* CParticleRain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleRain* pInstance = new CParticleRain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleRain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleRain::Free()
{
	__super::Free();
}

HRESULT CParticleRain::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Rain"),
		TEXT("Com_Component_Rain"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	return S_OK;
}

ParticleAttribute CParticleRain::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = {GetRandomFloat(-30.f, 30.f), 20.f, GetRandomFloat(-30.f, 30.f) };	
	att.vColor = { 0.1f, 0.f, 0.f, 1.f };
	att.vVelocity = { 0.f, GetRandomFloat(-180.f, -10.f), 0.f };

	return att;
}

void CParticleRain::OnBoundingExit(ParticleAttribute& particle)
{
	particle.vPosition = { GetRandomFloat(-30.f, 30.f), 20.f, GetRandomFloat(-30.f, 30.f) };
}

HRESULT CParticleRain::PrevRender()
{
	if (FAILED(__super::PrevRender()))
	{
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParticleRain::EndRender()
{
	if (FAILED(__super::EndRender()))
	{
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}
