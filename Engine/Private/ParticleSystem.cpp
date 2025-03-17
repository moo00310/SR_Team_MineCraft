#include "ParticleSystem.h"

CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device) : CGameObject(pGraphic_Device)
{
}

CParticleSystem::CParticleSystem(const CParticleSystem& Prototype) : 
	CGameObject(Prototype)		
{
	Safe_AddRef(m_pVB);		
	Safe_AddRef(m_pParticleTexture);
}

HRESULT CParticleSystem::Initialize(void* pArg)
{
 	if (FAILED(Create_VertexBuffer()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CParticleSystem::Initialize_Prototype()
{
	return S_OK;
}

void CParticleSystem::Update(_float fTimeDelta)
{
	for (auto& data : m_ListParticleAttribute)
	{
		data.vPosition += data.vVelocity * fTimeDelta;
	}
}

HRESULT CParticleSystem::Render()
{
	if (m_ListParticleAttribute.size() <= 0)
	{
		return S_OK;
	}

	if (FAILED(PrevRender()))
	{
		return E_FAIL;
	}

	Bind_Buffers();

	PARTICLE* p = 0;	

	// 버텍스 시작 사이즈.
	_uint startSize = 0;

	// 버텍스 인덱스 (포인트 스프라이트는 점 하나다).
	_uint vertexIndex = 0;

	for (auto& data : m_ListParticleAttribute)
	{
		// size에서 sizeof(PARTICLE) 크기만큼 할당.
		m_pVB->Lock(startSize, sizeof(PARTICLE), (void**)&p, D3DLOCK_NOOVERWRITE);

		p->Position = data.vPosition;
		p->Color = data.vColor;

		m_pVB->Unlock();

		// 점을 그린다.
		// vertexIndex부터 1개만 그린다 (점 하나기 때문에 버텍스도 한 개이다).
		m_pGraphic_Device->DrawPrimitive(
			D3DPT_POINTLIST,
			vertexIndex++,
			1);

		// 다음 시작 사이즈.
		startSize += sizeof(PARTICLE);
	}

	if (FAILED(EndRender()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CParticleSystem::Bind_Buffers()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(PARTICLE));

	/* 장치가 알아서 내 정점을 이용하여 특정 연산을 수행할 수 있도록 */
	/* 내 정점의 정보를 알려준다. */
	m_pGraphic_Device->SetFVF(m_iFVF);

	return S_OK;
}

HRESULT CParticleSystem::PrevRender()
{
	// 포인트 스프라이트 활성화.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	
	// 포인트 스프라이트 크기를 뷰 스페이스 단위로 해석.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	// 포인트 스프라이트 크기.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, dwPointSize);

	/*m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MIN, m_ParticleAttribute.dwPointSizeMin);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MAX, m_ParticleAttribute.dwPointSizeMax);*/

	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, dwPointScaleA);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, dwPointScaleB);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, dwPointScaleC);

	return S_OK;
}

HRESULT CParticleSystem::EndRender()
{
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);

	return S_OK;
}

HRESULT CParticleSystem::Create_VertexBuffer()
{
	m_iFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	m_pGraphic_Device->CreateVertexBuffer(
		iParticleCount * sizeof(PARTICLE),
		// 동적 버텍스 버퍼를 이용할 것이며 | 포인트 스프라이트이며 | 이건 잘 모르겠다
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		m_iFVF,
		// 동적 버텍스 버퍼는 MANAGED에 관리를 못한다고함, 그래서 디폴트줌.
		D3DPOOL_DEFAULT,
		&m_pVB,
		0
	);

	return S_OK;
}

void CParticleSystem::SetParticleAttribute()
{
	for (int i = 0; i < iParticleCount; i++)
	{
		ParticleAttribute att = AddParticle();		

		m_ListParticleAttribute.push_back(att);
	}	
}

DWORD CParticleSystem::GetScale(float f)
{
	return *((DWORD*)&f);
}

float CParticleSystem::GetRandomFloat(float lowBound, float highBound)
{
	// 잘못된 입력 
	if (lowBound >= highBound)
	{
		return lowBound;
	}

	// [0, 1] 범위의 임의의 float 획득.
	float f = (rand() % 10000) * 0.0001f;

	// 최종적으로 lowBound ~ highBound 범위 값 리턴.
	return (f * (highBound - lowBound)) + lowBound;
}

void CParticleSystem::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pParticleTexture);
}
