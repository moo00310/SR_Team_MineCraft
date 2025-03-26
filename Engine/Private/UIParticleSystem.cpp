#include "UIParticleSystem.h"

CUIParticleSystem::CUIParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CUIObject(pGraphic_Device)
{
}

CUIParticleSystem::CUIParticleSystem(const CUIParticleSystem& Prototype) :
	CUIObject(Prototype)
{
	ZeroMemory(&m_boundingBox, sizeof(ParticleBoundingBox));
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pParticleTexture);
	Safe_AddRef(m_pTransform);
}

HRESULT CUIParticleSystem::Initialize(void* pArg)
{
	// ���� ����.
	__super::Initialize(pArg);

	// ��ƼŬ �Ӽ� ����.
	SetParticleAttribute();

	return S_OK;
}

HRESULT CUIParticleSystem::Initialize_Prototype()
{
	return S_OK;
}

void CUIParticleSystem::Priority_Update(_float fTimeDelta)
{
}

void CUIParticleSystem::Update(_float fTimeDelta)
{
	for (auto& data : m_ListParticleAttribute)
	{
		// ��ƼŬ Ÿ�̸� Ȱ��ȭ �� �ð� ����.
		if (data.IsAlive == true && data.IsTime == true)
		{
			data.fCurrentTime += fTimeDelta;
		}

		// ��ƼŬ ���� Ȯ��.
		if (data.IsAlive == true && data.fCurrentTime >= data.fEndTime && data.IsTime == true)
		{
			data.IsAlive = false;
		}

		// ��輱 ����� ��ġ ����.
		if (IsBounding == true && m_boundingBox.IsExit(data.vPosition) == true)
		{
			OnBoundingExit(data);
		}

		// ��ƼŬ �߷� ����������?.
		if (data.IsAlive == true && data.IsGravity == true)
		{
			// �߷� ���� �� �߷� �ް�.
			float height = 0.f + data.fGravityJumpPower * data.fGravityTime - (1.f / 2.f) * data.fGravity * (data.fGravityTime * data.fGravityTime);
			data.fGravityTime += fTimeDelta;

			data.vPosition += data.vVelocity * fTimeDelta;
			data.vPosition.y += height;
		}
		else
		{
			// �߷� ���� �� �ܼ� �̵���.
			data.vPosition += data.vVelocity * fTimeDelta;
		}
	}
}

void CUIParticleSystem::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUIParticleSystem::Render()
{
	if (m_ListParticleAttribute.size() <= 0)
	{
		return S_OK;
	}

	if (FAILED(PrevRender()))
	{
		return E_FAIL;
	}

	if (m_pTransform == nullptr || FAILED(m_pTransform->Bind_Resource()))
	{
		return E_FAIL;
	}

	if (FAILED(m_pParticleTexture->Bind_Resource(0)))
	{
		return E_FAIL;
	}

	Bind_Buffers();

	// ��ƼŬ ���ؽ� ����.
	VTXPARTICLE* p = 0;

	// ���� ���׸�Ʈ�� ���ؽ� �ܰ�.
	_uint currentSegmentVertexIndex = 0;

	// ������ �ʱ�ȭ.
	dwVpOffset = 0;

	// dwVpOffset * sizeof(VTXPARTICLE)���� dwVpSBatchSize * sizeof(PARTICLE) ũ�⸸ŭ �Ҵ�.
	m_pVB->Lock(dwVpOffset * sizeof(VTXPARTICLE), dwVpBatchSize * sizeof(VTXPARTICLE), (void**)&p, D3DLOCK_NOOVERWRITE);

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
		currentSegmentVertexIndex++;

		// ���� ���׸�Ʈ �ܰ谡 �� ä������ ��� �׸���.
		if (currentSegmentVertexIndex == dwVpBatchSize)
		{
			// ��� ����.
			m_pVB->Unlock();

			// ���� ���� ���� �׸���.			
			m_pGraphic_Device->DrawPrimitive(
				D3DPT_POINTLIST,
				dwVpOffset,
				dwVpBatchSize);

			// ������ �߰�.
			dwVpOffset += dwVpBatchSize;

			// ���� ���׸�Ʈ �׸��� ���� ��.
			m_pVB->Lock(dwVpOffset * sizeof(VTXPARTICLE), dwVpBatchSize * sizeof(VTXPARTICLE), (void**)&p, D3DLOCK_NOOVERWRITE);

			// ���� ���׸�Ʈ�� �ܰ� �ʱ�ȭ.
			currentSegmentVertexIndex = 0;
		}
	}

	if (FAILED(EndRender()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUIParticleSystem::Bind_Buffers()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXPARTICLE));

	/* ��ġ�� �˾Ƽ� �� ������ �̿��Ͽ� Ư�� ������ ������ �� �ֵ��� */
	/* �� ������ ������ �˷��ش�. */
	m_pGraphic_Device->SetFVF(m_iFVF);

	return S_OK;
}

void CUIParticleSystem::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pParticleTexture);
	Safe_Release(m_pTransform);
}

void CUIParticleSystem::OnPushPool()
{
	// ����� �� ������ �� ���.
	for (auto& particle : m_ListParticleAttribute)
	{
		ParticleAttribute att = OnSetAddParticle();

		particle = att;
	}
}

void CUIParticleSystem::SetParticleAttribute()
{
	for (_uint i = 0; i < iParticleCount; i++)
	{
		ParticleAttribute att = OnSetAddParticle();

		m_ListParticleAttribute.push_back(att);
	}
}

HRESULT CUIParticleSystem::PrevRender()
{
	// ����Ʈ ��������Ʈ Ȱ��ȭ.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// ����Ʈ ��������Ʈ ũ�⸦ ��ũ�� �����̽� ������ �ؼ�.
	// �� ũ�⸦ 5�� ������ 5�ȼ��� �������ϰڴ� �ǹ��̴�.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);

	// ����Ʈ ��������Ʈ ũ��.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, dwPointSize);

	// �����ؽ��� Ȱ��ȭ�� ���� �ȿ� ����.
	__super::Begin();

	return S_OK;
}

HRESULT CUIParticleSystem::EndRender()
{
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);

	__super::End();

	return S_OK;
}

HRESULT CUIParticleSystem::Create_VertexBuffer()
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

void CUIParticleSystem::SetParticleBoundingBox(ParticleBoundingBox box)
{
	m_boundingBox = box;
}

DWORD CUIParticleSystem::GetScale(float f)
{
	return *((DWORD*)&f);
}

float CUIParticleSystem::GetRandomFloat(float lowBound, float highBound)
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

HRESULT CUIParticleSystem::Ready_Components()
{
	return S_OK;
}
