#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"

BEGIN(Client)

// ���� �Դ� �θ� ��ƼŬ.
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

	// �� ��ƼŬ �Ӽ��� �����մϴ�.	
	virtual ParticleAttribute OnSetAddParticle() override;	
};

END