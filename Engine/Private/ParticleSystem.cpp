#include "ParticleSystem.h"

CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device) : CGameObject(pGraphic_Device)
{
}

CParticleSystem::CParticleSystem(const CParticleSystem& Prototype) : 
	CGameObject(Prototype)
{
	ZeroMemory(&m_boundingBox, sizeof(ParticleBoundingBox));
	Safe_AddRef(m_pVB);		
	Safe_AddRef(m_pParticleTexture);
	Safe_AddRef(m_pTransform);
}

HRESULT CParticleSystem::Initialize(void* pArg)
{
	// ��ƼŬ �Ӽ� ����.
	SetParticleAttribute();

	fCurrentAnimationFrame = 0.f;
	iCurrentTextureIndex = 0;

	return S_OK;
}

HRESULT CParticleSystem::Initialize_Prototype()
{
	return S_OK;
}

void CParticleSystem::Priority_Update(_float fTimeDelta)
{
	if (IsTimer == true)
	{
		// �ð� ������ ����.
		if (m_fCurrentTimer >= fEndTimer)
		{
			SetActive(false);
			return;
		}

		m_fCurrentTimer += fTimeDelta;
	}
}

void CParticleSystem::Update(_float fTimeDelta)
{
	if (IsTextureAnimation == true && fCurrentAnimationFrame >= fAnimationFrame)
	{
		if (iCurrentTextureIndex >= iTextureIndex)
		{
			iCurrentTextureIndex = 0;
		}
		else
		{
			iCurrentTextureIndex++;
		}		
		fCurrentAnimationFrame = 0.f;
	}

	if (IsTextureAnimation == true)
	{
		fCurrentAnimationFrame += fTimeDelta;
	}

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

void CParticleSystem::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
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

	if (m_pTransform == nullptr || FAILED(m_pTransform->Bind_Resource()))
	{
		return E_FAIL;
	}

	if (FAILED(m_pParticleTexture->Bind_Resource(iCurrentTextureIndex)))
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

	m_pVB->Unlock();

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

void CParticleSystem::Replay(_float3 _position)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, _position);

	for (auto& particle : m_ListParticleAttribute)
	{
		particle.fCurrentTime = 0.f;
		particle.fGravityTime = 0.f;
		particle.IsAlive = true;

		OnReplay(particle);
	}
}

CTransform* CParticleSystem::GetTransform() const
{
	return m_pTransform;
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

	// ��ƼŬ�� ī�޶�κ��� �󸶳� �������ִ����� ���Ѹ�ŭ ũ�� (z��).
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, dwPointScaleA);

	// ��ƼŬ�� ī�޶�κ��� �Ÿ��� �󸶳� �������ִ����� ���Ѹ�ŭ ũ�� (x�� y��).
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, dwPointScaleB);

	// �Ÿ� ������� ��ƼŬ ũ�Ⱑ ������.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, dwPointScaleC);

	// �����ؽ��� Ȱ��ȭ.
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParticleSystem::EndRender()
{
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);

	// ���� �ؽ��� ��Ȱ��ȭ.
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CParticleSystem::Ready_Components()
{
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
	for (_uint i = 0; i < iParticleCount; i++)
	{
		ParticleAttribute att = OnSetAddParticle();

		m_ListParticleAttribute.push_back(att);
	}
}

void CParticleSystem::SetParticleBoundingBox(ParticleBoundingBox box)
{
	m_boundingBox = box;
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
	Safe_Release(m_pTransform);
}

void CParticleSystem::OnPushPool()
{
	iCurrentTextureIndex = 0;
	m_fCurrentTimer = 0.f;

	// ����� �� ������ �� ���.
	for (auto& particle : m_ListParticleAttribute)
	{
		ParticleAttribute att = OnSetAddParticle();

		particle = att;
	}
}
