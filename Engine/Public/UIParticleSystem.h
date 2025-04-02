#pragma once

#include "GameInstance.h"
#include "UIObject.h"
#include "Texture.h"
#include "Transform.h"

BEGIN(Engine)

// UI용 파티클.
class ENGINE_DLL CUIParticleSystem : public CUIObject
{
protected:
	CUIParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUIParticleSystem(const CUIParticleSystem& Prototype);
	virtual ~CUIParticleSystem() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	virtual HRESULT Render() override;
	HRESULT Bind_Buffers();

	virtual void Free() override;

	// 풀링에서 Push시 호출.
	virtual void OnPushPool() override;

private:
	// 파티클 속성 셋팅.
	void SetParticleAttribute();

protected:
	// 렌더링 전 상태 셋팅.
	virtual HRESULT PrevRender();

	// 렌더링 후 상태 셋팅.
	virtual HRESULT EndRender();

	HRESULT Create_VertexBuffer();

	// 파티클 경계선 셋팅.
	void SetParticleBoundingBox(ParticleBoundingBox box);

	// float 매개변수를 DWORD 형태로 반환.
	DWORD GetScale(float f);

	float GetRandomFloat(float lowBound, float highBound);

	// SetParticleAttribute 호출 시 실행.	
	// 파티클마다 각 속성을 정의 해준다.
	virtual ParticleAttribute OnSetAddParticle() = 0;

	virtual HRESULT Ready_Components();

	// 파티클 경계선 벗어나면 호출
	// 단, IsBounding이 true 상태여야 함.
	virtual void OnBoundingExit(ParticleAttribute& particle) = 0;

	// rgb형식을 dword로 변환.
	DWORD Float3ToHex(_float3 color);

protected:
	_uint	m_iFVF = {};
	LPDIRECT3DVERTEXBUFFER9 m_pVB = { nullptr };

	// 파티클 갯수.
	_uint iParticleCount = 0;

	// 파티클 세그먼트 당 채울 갯수.
	DWORD dwVpBatchSize;

	// 포인트 스프라이트 크기.
	DWORD dwPointSize;

	// 파티클 경계선 활성화.
	bool IsBounding = false;

	// 파티클 텍스쳐.
	CTexture* m_pParticleTexture = { nullptr };

	// 파티클 트랜스폼.
	CTransform* m_pTransform = { nullptr };

private:
	// 파티클 세그먼트 당 종료.
	DWORD dwVpOffset = 0;

	// 파티클 경계선.
	ParticleBoundingBox m_boundingBox;
	
	// 파티클 리스트.
	list<ParticleAttribute> m_ListParticleAttribute;
};

END