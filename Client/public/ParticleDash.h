#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"

// 달리기 파티클.
class CParticleDash : public CParticleSystem
{
private:
	CParticleDash(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleDash(const CParticleDash& Prototype);
	virtual ~CParticleDash() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;	

	// CParticleSystem을(를) 통해 상속됨
	CGameObject* Clone(void* pArg) override;
	static CParticleDash* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();

protected:
	virtual HRESULT Ready_Components();

	// 각 파티클 속성을 셋팅합니다.	
	virtual ParticleAttribute OnSetAddParticle() override;

	// 파티클 경계선(IsBounding)을 활성화 한 상태에서
	// m_boundingBox로 지정한 범위를 넘어서면 호출.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;
};

