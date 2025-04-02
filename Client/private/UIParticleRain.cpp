#include "UIParticleRain.h"

// UI용 비 파티클.
CUIParticleRain::CUIParticleRain(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CUIParticleSystem(pGraphic_Device)
{
}

CUIParticleRain::CUIParticleRain(const CUIParticleRain& Prototype) :
	CUIParticleSystem(Prototype)
{
}

HRESULT CUIParticleRain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIParticleRain::Initialize(void* pArg)
{
	iParticleCount = 50;
	IsBounding = true;

    UIOBJECT_DESC Desc{};

    Desc.fSizeX = g_iWinSizeX;
    Desc.fSizeY = g_iWinSizeY;
    Desc.fX = g_iWinSizeX * 0.5f;
    Desc.fY = g_iWinSizeY * 0.5f;

	if (FAILED(__super::Initialize(&Desc)))
	{
		return E_FAIL;
	}	

	dwVpBatchSize = 50;
	dwPointSize = GetScale(300.f);	// 포인트 스프라이트 크기.

	ParticleBoundingBox box;
	box.vMinPosition = { -2000.f, -500.f, -30.f };		// 최소 범위.
	box.vMaxPosition = { 2000.f, 800.f, 30.f };		// 최대 범위.

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

CUIParticleRain* CUIParticleRain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CUIParticleRain* pInstance = new CUIParticleRain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUIParticleRain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIParticleRain::Clone(void* pArg)
{
	CUIParticleRain* pInstance = new CUIParticleRain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CUIParticleRain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

ParticleAttribute CUIParticleRain::OnSetAddParticle()
{
	ParticleAttribute att;
	att.vPosition = { GetRandomFloat(float(0.f - g_iWinSizeX), float(g_iWinSizeX)), GetRandomFloat(200.f, 700.f), 0.f};
	att.vColor = Float3ToHex({ 0.f, 0.f, 1.f });
	//att.vVelocity = { 0.f, GetRandomFloat(-1200.f, -900.f), 0.f };	
	att.vVelocity = { 0.f, GetRandomFloat(-800.f, -500.f), 0.f };
	att.IsAlive = true;

	return att;
}

HRESULT CUIParticleRain::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_RAIN,
		TEXT("Com_Component_Rain"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Component_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CUIParticleRain::OnBoundingExit(ParticleAttribute& particle)
{
	particle.vPosition = { GetRandomFloat(float(0.f - g_iWinSizeX), float(g_iWinSizeX)), 500.f, 0.f };
}
