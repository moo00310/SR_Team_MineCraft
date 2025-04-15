#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"

BEGIN(Client)

// 음식 먹는 부모 파티클.
class CBaseParticleEating abstract : public CParticleSystem
{
protected:
	CBaseParticleEating(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBaseParticleEating(const CBaseParticleEating& Prototype);
	virtual ~CBaseParticleEating() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;	
	virtual void OnPushPool() override;

protected:
	virtual HRESULT Ready_Components() override;

	// 각 파티클 속성을 셋팅합니다.	
	virtual ParticleAttribute OnSetAddParticle() override;	
};

END