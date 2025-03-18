#pragma once
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
	virtual void Update(_float fTimeDelta) override;

	virtual HRESULT Render() override;
	HRESULT Bind_Buffers();

protected:
	// 렌더링 전 상태 셋팅.
	virtual HRESULT PrevRender();

	// 렌더링 후 상태 셋팅.
	virtual HRESULT EndRender();

protected:
	HRESULT Create_VertexBuffer();

	// 파티클 속성 셋팅.
	void SetParticleAttribute();	

	// float 매개변수를 DWORD 형태로 반환.
	DWORD GetScale(float f);

	float GetRandomFloat(float lowBound, float highBound);

	virtual ParticleAttribute AddParticle() = 0;

public:
	virtual void Free() override;

private:
	// 파티클 세그먼트 당 종료.
	DWORD dwVpOffset = 0;

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
	DWORD dwPointScaleA;

	// 포인트 스프라이트 거리별 크기 B.
	DWORD dwPointScaleB;

	// 포인트 스프라이트 거리별 크기 C.
	DWORD dwPointScaleC;

	// 파티클 텍스쳐.
	CTexture* m_pParticleTexture = { nullptr };
	
private:
	list<ParticleAttribute> m_ListParticleAttribute;
};

END;
