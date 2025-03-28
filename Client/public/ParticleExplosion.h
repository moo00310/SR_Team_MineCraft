#pragma once
#include "Client_Defines.h"
#include "ParticleSystem.h"

BEGIN(Client)

class CParticleExplosion final : public CParticleSystem
{
private:
	CParticleExplosion(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleExplosion(const CParticleExplosion& Prototype);
	virtual ~CParticleExplosion() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	// CParticleSystem을(를) 통해 상속됨
	CGameObject* Clone(void* pArg) override;
	static CParticleExplosion* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();

protected:
	virtual HRESULT Ready_Components() override;

	// 각 파티클 속성을 셋팅합니다.	
	virtual ParticleAttribute OnSetAddParticle() override;

	// CParticleSystem을(를) 통해 상속됨
	void OnBoundingExit(ParticleAttribute& particle) override;
};

END