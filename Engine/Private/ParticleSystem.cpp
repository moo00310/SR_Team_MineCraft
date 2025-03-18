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
		if (data.IsAlive == true && data.IsTime == true)
		{
			data.fCurrentTime += fTimeDelta;
		}

		if (data.IsAlive == true && data.fCurrentTime >= data.fEndTime && data.IsTime == true)
		{
			data.IsAlive = false;
		}

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

	// ��ƼŬ ���ؽ� ����.
	VTXPARTICLE* p = 0;

	// ���� ���׸�Ʈ�� ���ؽ� �ܰ�.
	_uint currentVertexIndex = 0;	

	// ������ �ʱ�ȭ.
	dwVpOffset = 0;

	// dwVpOffset * sizeof(VTXPARTICLE)���� dwVpSBatchSize * sizeof(PARTICLE) ũ�⸸ŭ �Ҵ�.
	m_pVB->Lock(dwVpOffset * sizeof(VTXPARTICLE), dwVpSBatchSize * sizeof(VTXPARTICLE), (void**)&p, D3DLOCK_NOOVERWRITE);

	// ��ƼŬ �ν��Ͻ� ����.
	for (auto& data : m_ListParticleAttribute)
	{
		// ������ ��ƼŬ���� Ȯ��.
		if (data.IsAlive == false)
		{
			continue;
		}

		p->Position = data.vPosition;
		p->Color = data.vColor;

		// ���� ���ؽ��� ����.
		p++;

		// ���� ���׸�Ʈ�� �ܰ� ����.
		currentVertexIndex++;

		// ���� ���׸�Ʈ �ܰ谡 �� ä������ ��� �׸���.
		if (currentVertexIndex == dwVpSBatchSize)
		{
			// ��� ����.
			m_pVB->Unlock();

			// ���� ���� ���� �׸���.			
			m_pGraphic_Device->DrawPrimitive(
				D3DPT_POINTLIST,
				dwVpOffset,
				dwVpSBatchSize);			

			// ������ �߰�.
			dwVpOffset += dwVpSBatchSize;

			// ���� ���׸�Ʈ �׸��� ���� ��.
			m_pVB->Lock(dwVpOffset * sizeof(VTXPARTICLE), dwVpSBatchSize * sizeof(VTXPARTICLE), (void**)&p, D3DLOCK_NOOVERWRITE);

			// ���� ���׸�Ʈ�� �ܰ� �ʱ�ȭ.
			currentVertexIndex = 0;
		}		
	}	

	if (FAILED(EndRender()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CParticleSystem::Bind_Buffers()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXPARTICLE));

	/* ��ġ�� �˾Ƽ� �� ������ �̿��Ͽ� Ư�� ������ ������ �� �ֵ��� */
	/* �� ������ ������ �˷��ش�. */
	m_pGraphic_Device->SetFVF(m_iFVF);

	return S_OK;
}

HRESULT CParticleSystem::PrevRender()
{
	// ����Ʈ ��������Ʈ Ȱ��ȭ.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	
	// ����Ʈ ��������Ʈ ũ�⸦ �� �����̽� ������ �ؼ�.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	// ����Ʈ ��������Ʈ ũ��.
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
		iParticleCount * sizeof(VTXPARTICLE),
		// ���� ���ؽ� ���۸� �̿��� ���̸� | ����Ʈ ��������Ʈ�̸� | �̰� �� �𸣰ڴ�
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		m_iFVF,
		// ���� ���ؽ� ���۴� MANAGED�� ������ ���Ѵٰ���, �׷��� ����Ʈ��.
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
		att.IsAlive = true;

		m_ListParticleAttribute.push_back(att);
	}	
}

DWORD CParticleSystem::GetScale(float f)
{
	return *((DWORD*)&f);
}

float CParticleSystem::GetRandomFloat(float lowBound, float highBound)
{
	// �߸��� �Է� 
	if (lowBound >= highBound)
	{
		return lowBound;
	}

	// [0, 1] ������ ������ float ȹ��.
	float f = (rand() % 10000) * 0.0001f;

	// ���������� lowBound ~ highBound ���� �� ����.
	return (f * (highBound - lowBound)) + lowBound;
}

void CParticleSystem::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pParticleTexture);
}
