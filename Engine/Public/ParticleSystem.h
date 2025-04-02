#pragma once

#include "GameInstance.h"
#include "GameObject.h"
#include "Texture.h"

BEGIN(Engine)

class ENGINE_DLL CParticleSystem abstract : public CGameObject
{
protected:
	CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleSystem(const CParticleSystem& Prototype);
	virtual ~CParticleSystem() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Initialize_Prototype() override;
	virtual void Priority_Update(_float fTimeDelta) override;	
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	virtual HRESULT Render() override;
	HRESULT Bind_Buffers();

	// 파티클 속성들 초기화.
	void Replay(_float3 _position);

	CTransform* GetTransform() const;

protected:
	// 렌더링 전 상태 셋팅.
	virtual HRESULT PrevRender();

	// 렌더링 후 상태 셋팅.
	virtual HRESULT EndRender();

	virtual void OnReplay(ParticleAttribute& particle) {};

	virtual HRESULT Ready_Components();

protected:
	HRESULT Create_VertexBuffer();

private:
	// 파티클 속성 셋팅.
	void SetParticleAttribute();	

protected:
	// 파티클 경계선 셋팅.
	void SetParticleBoundingBox(ParticleBoundingBox box);

	// float 매개변수를 DWORD 형태로 반환.
	DWORD GetScale(float f);

	float GetRandomFloat(float lowBound, float highBound);

	// SetParticleAttribute 호출 시 실행.	
	// 파티클마다 각 속성을 정의 해준다.
	virtual ParticleAttribute OnSetAddParticle() = 0;

	// 파티클 경계선 벗어나면 호출
	// 단, IsBounding이 true 상태여야 함.
	virtual void OnBoundingExit(ParticleAttribute& particle) = 0;

	// rgb형식을 dword로 변환.
	DWORD Float3ToHex(_float3 color);

public:
	virtual void Free() override;

	// 풀링에서 Push시 호출.
	virtual void OnPushPool() override;

private:
	// 파티클 세그먼트 당 종료.
	DWORD dwVpOffset = 0;

	// 파티클 경계선.
	ParticleBoundingBox m_boundingBox;

	// 텍스쳐 애니메이션 현재 프레임 시간.
	float fCurrentAnimationFrame;	

	// 파티클 현재 시간.
	float m_fCurrentTimer = { 0.f };

protected:
	_uint	m_iFVF = {};
	LPDIRECT3DVERTEXBUFFER9 m_pVB = { nullptr };
	
	// 파티클 갯수.
	_uint iParticleCount = 0;

	// 파티클 세그먼트 당 채울 갯수.
	DWORD dwVpBatchSize;	

	// 포인트 스프라이트 크기.
	DWORD dwPointSize;

	// 포인트 스프라이트 최소 크기.
	DWORD dwPointSizeMin;

	// 포인트 스프라이트 최대 크기.
	DWORD dwPointSizeMax;

	// 포인트 스프라이트 거리별 크기 A.
	// 값이 높을수록 카메라랑 멀어지면 파티클은 크게 보임.
	DWORD dwPointScaleA;

	// 포인트 스프라이트 거리별 크기 B.
	// 값이 높을수록 카메라랑 멀어지면 파티클이 작아보이게 함.
	DWORD dwPointScaleB;

	// 포인트 스프라이트 거리별 크기 C.
	// 값이 높을수록 실제 거리에 따른 파티클 크기가 보정됨.
	DWORD dwPointScaleC;	

	// 파티클 경계선 활성화.
	bool IsBounding = false;

	// 텍스쳐 애니메이션 활성화.
	bool IsTextureAnimation = false;

	// 텍스쳐 애니메이션 프레임 시간.
	float fAnimationFrame;

	// 파티클 객체 타이머 활성화.
	bool IsTimer = false;	

	// 파티클 종료 시간.
	float fEndTimer = { 0.f };

	// 텍스쳐 마지막 인덱스.
	_uint iTextureIndex;

	// 텍스쳐 현재 인덱스.
	_uint iCurrentTextureIndex;

	// 파티클 입자 텍스쳐들을 각각 다르게 적용 시킬 것인가?.
	// 이 기능을 활성화 시킬경우 해당 파티클은
	// 인스턴싱 기능이 적용이 안되니
	// 너무 많은 파티클 입자에는 적용하지 말 것.
	bool IsParticleTexture = {false};

	// 파티클 텍스쳐.
	CTexture* m_pParticleTexture = { nullptr };

	// 파티클 트랜스폼.
	CTransform* m_pTransform = { nullptr };

	// 파티클 쉐이더.
	CShader* m_pShader = {nullptr};
	
private:
	list<ParticleAttribute> m_ListParticleAttribute;
};

END;
