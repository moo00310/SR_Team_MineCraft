#include "TestParticle.h"

CTestParticle::CTestParticle(LPDIRECT3DDEVICE9 pGraphic_Device) : CParticleSystem(pGraphic_Device)
{
}

CTestParticle::CTestParticle(const CTestParticle& Prototype) : CParticleSystem(Prototype)
{	
}

HRESULT CTestParticle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTestParticle::Initialize(void* pArg)
{
	// ��ƼŬ �Ӽ� ���� �۾�.
	IsBounding = true;			// ���� ����.
	iParticleCount = 500;		// ��ƼŬ ����.
	dwVpBatchSize = 250;			// ���׸�Ʈ �� ������.
	dwPointSize = GetScale(2.f);	// ����Ʈ ��������Ʈ ũ��.
	dwPointScaleA = GetScale(0.f);	// ����Ʈ ��������Ʈ �Ÿ��� ũ��.
	dwPointScaleB = GetScale(0.f);
	dwPointScaleC = GetScale(1.f);			

	// ��ƼŬ �Ӽ� ����.
	SetParticleAttribute();

	// ��ƼŬ ��輱 ���� (����).
	ParticleBoundingBox box;
	box.vMinPosition = { -20.f, -5.f, -20.f };		// �ּ� ����.
	box.vMaxPosition = { 20.f, 20.f, 20.f };		// �ִ� ����.

	// ��ƼŬ ��輱 ���� �۾�.
	SetParticleBoundingBox(box);

	// SetParticleAttribute �ϰ� �ݵ�� 
	// �θ� Initialize ȣ����Ѽ� ���ؽ� �ʱ�ȭ ��ų ��.
	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}		

	return S_OK;
}

void CTestParticle::Priority_Update(_float fTimeDelta)
{
}

void CTestParticle::Update(_float fTimeDelta)
{	
	__super::Update(fTimeDelta);
}

void CTestParticle::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CTestParticle::Render()
{
	m_pParticleTexture->Bind_Resource(0);

	__super::Render();

	return S_OK;
}

CGameObject* CTestParticle::Clone(void* pArg)
{
	CTestParticle* pInstance = new CTestParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CTestParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTestParticle* CTestParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTestParticle* pInstance = new CTestParticle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTestParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTestParticle::Free()
{
	__super::Free();	
}

HRESULT CTestParticle::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Rain"),
		TEXT("Com_Component_Smoke"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	return S_OK;
}

// SetParticleAttribute ȣ�� �� �ݹ�.
// ��ƼŬ���� �� �Ӽ��� ���� ���ش�.
ParticleAttribute CTestParticle::OnSetAddParticle()
{	
	ParticleAttribute att;
	att.vPosition = { GetRandomFloat(-15.f, 15.f), 15.f, GetRandomFloat(-5.f, 5.f) };
	att.vColor = { 1.f, 1.f, 1.f, 1.f };
	att.vVelocity = { 0.f, -GetRandomFloat(20.f, 40.f), 0.f};
	/*att.fCurrentTime = 0.f;
	att.fEndTime = 3.f;
	att.IsTime = true;*/

	return att;
}

void CTestParticle::OnBoundingExit(ParticleAttribute& particle)
{
	particle.vPosition = { GetRandomFloat(-15.f, 15.f), 15.f, GetRandomFloat(-5.f, 5.f) };
}

HRESULT CTestParticle::PrevRender()
{
	if (FAILED(__super::PrevRender()))
	{
		return E_FAIL;
	}

	// �� �� �ؽ��Ĵ� ���� ���� �� ������.
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 155);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CTestParticle::EndRender()
{
	if (FAILED(__super::PrevRender()))
	{
		return E_FAIL;
	}

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}
