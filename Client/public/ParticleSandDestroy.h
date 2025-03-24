#pragma once
#include "BaseParticleDestroy.h"
#include "Client_Defines.h"

BEGIN(Client)

// 흙 파괴 될 때 파티클.
class CParticleSandDestroy : public CBaseParticleDestroy
{
private:
	CParticleSandDestroy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleSandDestroy(const CParticleSandDestroy& Prototype);
	virtual ~CParticleSandDestroy() = default;

public:
	virtual HRESULT Render() override;

	// CParticleSystem을(를) 통해 상속됨
	CGameObject* Clone(void* pArg) override;
	static CParticleSandDestroy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

protected:
	virtual HRESULT Ready_Components() override;
};

END