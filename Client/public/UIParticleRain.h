#pragma once
#include "Client_Defines.h"
#include "UIParticleSystem.h"

BEGIN(Client)

class CUIParticleRain : public CUIParticleSystem
{
private:
	CUIParticleRain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUIParticleRain(const CUIParticleRain& Prototype);
	virtual ~CUIParticleRain() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;

	static CUIParticleRain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;

protected:
	virtual HRESULT Ready_Components() override;

	// SetParticleAttribute 호출 시 실행.	
	// 파티클마다 각 속성을 정의 해준다.
	virtual ParticleAttribute OnSetAddParticle() override;	

	// 파티클 경계선 벗어나면 호출
	// 단, IsBounding이 true 상태여야 함.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;
};

END