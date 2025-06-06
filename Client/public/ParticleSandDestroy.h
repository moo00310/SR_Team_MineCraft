#pragma once
#include "BaseParticleDestroy.h"
#include "Client_Defines.h"

BEGIN(Client)

// �� �ı� �� �� ��ƼŬ.
class CParticleSandDestroy : public CBaseParticleDestroy
{
private:
	CParticleSandDestroy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleSandDestroy(const CParticleSandDestroy& Prototype);
	virtual ~CParticleSandDestroy() = default;

public:
	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleSandDestroy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void OnPushPool() override;

protected:
	virtual HRESULT Ready_Components() override;
};

END