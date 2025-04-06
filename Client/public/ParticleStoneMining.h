#pragma once
#include "BaseParticleMining.h"

BEGIN(Client)

class CParticleStoneMining : public CBaseParticleMining
{
private:
	CParticleStoneMining(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleStoneMining(const CParticleStoneMining& Prototype);
	virtual ~CParticleStoneMining() = default;

public:
	// CParticleSystem을(를) 통해 상속됨
	CGameObject* Clone(void* pArg) override;
	static CParticleStoneMining* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void OnPushPool() override;

protected:
	virtual HRESULT Ready_Components() override;
};

END