#pragma once
#include "BaseParticleDestroy.h"
#include "Client_Defines.h"

BEGIN(Client)

class CParticleStoneDestroy : public CBaseParticleDestroy
{
private:
	CParticleStoneDestroy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleStoneDestroy(const CParticleStoneDestroy& Prototype);
	virtual ~CParticleStoneDestroy() = default;

public:
	// CParticleSystem을(를) 통해 상속됨
	CGameObject* Clone(void* pArg) override;
	static CParticleStoneDestroy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void OnPushPool() override;

protected:
	virtual HRESULT Ready_Components() override;
};

END