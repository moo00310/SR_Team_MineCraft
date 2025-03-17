#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CParticleSystem abstract : public CGameObject
{
protected:
	CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleSystem(const CParticleSystem& Prototype);
	virtual ~CParticleSystem() = default;

public:
	virtual HRESULT Render() override;
	HRESULT Bind_Buffers();

private:
	// 렌더링 전 상태 셋팅.
	virtual HRESULT PrevRender() = 0;

	// 렌더링 후 상태 셋팅.
	virtual HRESULT EndRender() = 0;

protected:
	HRESULT Create_VertexBuffer();

public:
	virtual void Free() override;

protected:
	_uint	m_iFVF = {};
	LPDIRECT3DVERTEXBUFFER9 m_pVB = { nullptr };	

	_uint m_iParticleCount = 0;

	DWORD vbSize;
	DWORD vbOffset;
	DWORD vbBatchSize;
};

END;
