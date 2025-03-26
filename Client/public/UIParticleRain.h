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

	// SetParticleAttribute ȣ�� �� ����.	
	// ��ƼŬ���� �� �Ӽ��� ���� ���ش�.
	virtual ParticleAttribute OnSetAddParticle() override;	

	// ��ƼŬ ��輱 ����� ȣ��
	// ��, IsBounding�� true ���¿��� ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;
};

END