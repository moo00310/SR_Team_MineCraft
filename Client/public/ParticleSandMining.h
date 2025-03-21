#pragma once
#include "BaseParticleMining.h"

BEGIN(Client)

class CParticleSandMining final : public CBaseParticleMining
{
private:	
	CParticleSandMining(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleSandMining(const CParticleSandMining& Prototype);
	virtual ~CParticleSandMining() = default;

public:
	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleSandMining* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

protected:
	virtual HRESULT Ready_Components() override;
};

END