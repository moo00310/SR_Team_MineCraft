#include "ParticleSystem.h"

CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device) : CGameObject(pGraphic_Device)
{
}

CParticleSystem::CParticleSystem(const CParticleSystem& Prototype) : CGameObject(Prototype)
{
	Safe_AddRef(m_pVB);	
}

HRESULT CParticleSystem::Render()
{
	if (FAILED(PrevRender()))
	{
		return E_FAIL;
	}

	Bind_Buffers();

	PARTICLE* p = 0;	

	m_pVB->Lock(0, 0, (void**)&p, D3DLOCK_NOOVERWRITE);

	p->Position = {0.f, 1.f, 0.f};
	p->Color = { 1.f, 1.f, 1.f, 1.f };

	m_pVB->Unlock();

	m_pGraphic_Device->DrawPrimitive(
		D3DPT_POINTLIST,
		0,
		1);

	m_pVB->Lock(0, 0, (void**)&p, D3DLOCK_NOOVERWRITE);

	p->Position = { 3.f, 1.f, 0.f };
	p->Color = { 1.f, 1.f, 1.f, 1.f };

	m_pVB->Unlock();

	m_pGraphic_Device->DrawPrimitive(
		D3DPT_POINTLIST,
		1,
		1);

	/*vbOffset += vbBatchSize;

	if (vbOffset >= vbSize)
		vbOffset = 0;*/

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

HRESULT CParticleSystem::Create_VertexBuffer()
{
	m_iFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	m_pGraphic_Device->CreateVertexBuffer(
		m_iParticleCount * sizeof(PARTICLE),
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

void CParticleSystem::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
}
