#pragma once
#include "GameInstance.h"
#include "ParticleSystem.h"
#include "Client_Defines.h"

class CTestParticle final : public CParticleSystem
{
private:
	CTestParticle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTestParticle(const CTestParticle& Prototype);
	virtual ~CTestParticle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;		

	// CParticleSystem을(를) 통해 상속됨
	CGameObject* Clone(void* pArg) override;
	static CTestParticle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();

protected:
	virtual HRESULT Ready_Components();
	virtual ParticleAttribute OnSetAddParticle() override;
	virtual void OnBoundingExit(ParticleAttribute& particle) override;

private:
	HRESULT PrevRender() override;
	HRESULT EndRender() override;	
};

