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
	// 직교 셋팅.
	__super::Initialize(pArg);

	// 파티클 속성 셋팅.
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

	if (FAILED(EndRender()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUIParticleSystem::Bind_Buffers()
{
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, sizeof(VTXPARTICLE));

	/* 장치가 알아서 내 정점을 이용하여 특정 연산을 수행할 수 있도록 */
	/* 내 정점의 정보를 알려준다. */
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
	// 재생성 시 기존에 값 덮어씀.
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
	// 포인트 스프라이트 활성화.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// 포인트 스프라이트 크기를 스크린 스페이스 단위로 해석.
	// 즉 크기를 5로 잡으면 5픽셀로 나오게하겠단 의미이다.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);

	// 포인트 스프라이트 크기.
	m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, dwPointSize);

	// 알파텍스쳐 활성화가 여기 안에 있음.
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

HRESULT CUIParticleSystem::Ready_Components()
{
	return S_OK;
}
