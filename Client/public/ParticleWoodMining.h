#pragma once
#include "BaseParticleMining.h"

BEGIN(Client)

class CParticleWoodMining final : public CBaseParticleMining
{
private:
	CParticleWoodMining(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleWoodMining(const CParticleWoodMining& Prototype);
	virtual ~CParticleWoodMining() = default;

public:
	// CParticleSystem을(를) 통해 상속됨
	CGameObject* Clone(void* pArg) override;
	static CParticleWoodMining* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

protected:
	virtual HRESULT Ready_Components() override;
};

END