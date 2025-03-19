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
	iParticleCount = 800;			// 파티클 갯수.

	// 항상 파티클 갯수를 먼저 정해주고 부모 Init 해줄 것.
	// 왜냐면 내부적으로 저 파티클 갯수 변수를 씀.
	if (FAILED(__super::Initialize(pArg)))
	{
		return E_FAIL;
	}

	// 파티클 속성 셋팅 작업.	
	dwVpBatchSize = 800;			// 세그먼트 당 사이즈.
	dwPointSize = GetScale(2.f);	// 포인트 스프라이트 크기.
	dwPointScaleA = GetScale(0.f);	// 포인트 스프라이트 거리별 크기.
	dwPointScaleB = GetScale(0.f);
	dwPointScaleC = GetScale(1.f);	

	// 파티클 경계선 셋팅 (범위).
	ParticleBoundingBox box;
	box.vMinPosition = { -5.f, -5.f, -5.f };		// 최소 범위.
	box.vMaxPosition = { 5.f, 5.f, 5.f };		// 최대 범위.

	// 파티클 경계선 셋팅 작업.
	SetParticleBoundingBox(box);
	
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
	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Smoke"),
		TEXT("Com_Component_Smoke"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	return S_OK;
}

// 부모 Initialize 호출 시 콜백.
// 파티클마다 각 속성을 정의 해준다.
ParticleAttribute CTestParticle::OnSetAddParticle()
{	
	ParticleAttribute att;
	att.vPosition = { 0.f, 0.f, 0.f };
	att.vColor = { GetRandomFloat(0.1f, 0.5f), 0.f, 0.f, 1.f };
	att.vVelocity = { GetRandomFloat(-5.f, 5.f), GetRandomFloat(-5.f, 5.f), GetRandomFloat(-5.f, 5.f) };
	att.fCurrentTime = 0.f;
	att.fEndTime = 2.f;
	att.IsTime = true;

	return att;
}

void CTestParticle::OnBoundingExit(ParticleAttribute& particle)
{	
	//particle.vPosition = { 0.f, 0.f, 0.f };
}

HRESULT CTestParticle::PrevRender()
{
	if (FAILED(__super::PrevRender()))
	{
		return E_FAIL;
	}	

	// 금 간 텍스쳐는 투명도 적용 후 렌더링.
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
