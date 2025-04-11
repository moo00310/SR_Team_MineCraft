#pragma once
#include "BaseParticleMining.h"

BEGIN(Client)

class CParticleLeafMining final : public CBaseParticleMining
{
private:
	CParticleLeafMining(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleLeafMining(const CParticleLeafMining& Prototype);
	virtual ~CParticleLeafMining() = default;

public:
	// CParticleSystem을(를) 통해 상속됨
	CGameObject* Clone(void* pArg) override;
	static CParticleLeafMining* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void OnPushPool() override;

protected:
	virtual HRESULT Ready_Components() override;
};

END