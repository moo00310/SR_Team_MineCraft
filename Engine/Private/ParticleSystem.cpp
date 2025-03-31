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
	// 파티클 속성 셋팅.
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
		// 시간 지나면 종료.
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
		// 파티클 타이머 활성화 시 시간 증가.
		if (data.IsAlive == true && data.IsTime == true)
		{
			data.fCurrentTime += fTimeDelta;
		}

		// 파티클 종료 확인.
		if (data.IsAlive == true && data.fCurrentTime >= data.fEndTime && data.IsTime == true)
		{
			data.IsAlive = false;
		}

		// 경계선 벗어나면 위치 리셋.
		if (IsBounding == true && m_boundingBox.IsExit(data.vPosition) == true)
		{
			OnBoundingExit(data);
		}

		// 파티클 중력 적용중인지?.
		if (data.IsAlive == true && data.IsGravity == true)
		{
			// 중력 적용 시 중력 받게.
			float height = 0.f + data.fGravityJumpPower * data.fGravityTime - (1.f / 2.f) * data.fGravity * (data.fGravityTime * data.fGravityTime);
			data.fGravityTime += fTimeDelta;

			data.vPosition += data.vVelocity * fTimeDelta;
			data.vPosition.y += height;			
		}
		else
		{
			// 중력 없을 시 단순 이동만.
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

	// 파티클 버텍스 버퍼.
	VTXPARTICLE* p = 0;

	// 현재 세그먼트의 버텍스 단계.
	_uint currentSegmentVertexIndex = 0;

	// 오프셋 초기화.
	dwVpOffset = 0;

	// dwVpOffset * sizeof(VTXPARTICLE)에서 dwVpSBatchSize * sizeof(PARTICLE) 크기만큼 할당.
	m_pVB->Lock(dwVpOffset * sizeof(VTXPARTICLE), dwVpBatchSize * sizeof(VTXPARTICLE), (void**)&p, D3DLOCK_NOOVERWRITE);

	// 파티클 인스턴싱 로직.
	for (auto& data : m_ListParticleAttribute)
	{
		// 생존한 파티클인지 확인.
		if (data.IsAlive == false)
		{
			continue;
		}

		p->Position = data.vPosition;
		p->Color = data.vColor;

		// 다음 버텍스로 증가.
		p++;

		// 현재 세그먼트의 단계 증가.
		currentSegmentVertexIndex++;

		// 현재 세그먼트 단계가 다 채워졌을 경우 그린다.
		if (currentSegmentVertexIndex == dwVpBatchSize)
		{
			// 잠금 해제.
			m_pVB->Unlock();

			// 지정 범위 점을 그린다.			
			m_pGraphic_Device->DrawPrimitive(
				D3DPT_POINTLIST,
				dwVpOffset,
				dwVpBatchSize);

			// 오프셋 추가.
			dwVpOffset += dwVpBatchSize;

			// 다음 세그먼트 그리기 위한 락.
			m_pVB->Lock(dwVpOffset * sizeof(VTXPARTICLE), dwVpBatchSize * sizeof(VTXPARTICLE), (void**)&p, D3DLOCK_NOOVERWRITE);

			// 현재 세그먼트의 단계 초기화.
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

	/* 장치가 알아서 내 정점을 이용하여 특정 연산을 수행할 수 있도록 */
	/* 내 정점의 정보를 알려준다. */
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
	// 포인트 스프라이트 활성화.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	
	// 포인트 스프라이트 크기를 뷰 스페이스 단위로 해석.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	// 포인트 스프라이트 크기.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, dwPointSize);

	/*m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MIN, m_ParticleAttribute.dwPointSizeMin);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MAX, m_ParticleAttribute.dwPointSizeMax);*/

	// 파티클이 카메라로부터 얼마나 떨어져있는지에 대한만큼 크기 (z축).
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, dwPointScaleA);

	// 파티클이 카메라로부터 거리가 얼마나 떨어져있는지에 대한만큼 크기 (x랑 y축).
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, dwPointScaleB);

	// 거리 상관없이 파티클 크기가 일정함.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, dwPointScaleC);

	// 알파텍스쳐 활성화.
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CParticleSystem::EndRender()
{
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);

	// 알파 텍스쳐 비활성화.
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
	Safe_Release(m_pTransform);
}

void CParticleSystem::OnPushPool()
{
	iCurrentTextureIndex = 0;
	m_fCurrentTimer = 0.f;

	// 재생성 시 기존에 값 덮어씀.
	for (auto& particle : m_ListParticleAttribute)
	{
		ParticleAttribute att = OnSetAddParticle();

		particle = att;
	}
}
