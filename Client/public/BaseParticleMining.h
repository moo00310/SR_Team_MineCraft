#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"
#include "Sun.h"

BEGIN(Client)

// 블럭 채굴 부모 파티클.
class CBaseParticleMining abstract : public CParticleSystem
{
protected:
	CBaseParticleMining(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBaseParticleMining(const CBaseParticleMining& Prototype);
	virtual ~CBaseParticleMining() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	// CParticleSystem을(를) 통해 상속됨	
	virtual void Free();

	virtual void OnPushPool() override;

protected:
	virtual HRESULT Ready_Components() = 0;

	// 각 파티클 속성을 셋팅합니다.
	virtual ParticleAttribute OnSetAddParticle() override;

	// 파티클 경계선(IsBounding)을 활성화 한 상태에서
	// m_boundingBox로 지정한 범위를 넘어서면 호출.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;

	virtual void OnReplay(ParticleAttribute& particle) override;

private:
	CSun* m_Sun = { nullptr };
};

END