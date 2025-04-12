#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"

BEGIN(Client)

// ȭ�� �� ��ƼŬ.
class CParticleFurnace final : public CParticleSystem
{
private:
	CParticleFurnace(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleFurnace(const CParticleFurnace& Prototype);
	virtual ~CParticleFurnace() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleFurnace* Create(LPDIRECT3DDEVICE9 pGraphic_Device);	

protected:
	virtual HRESULT Ready_Components() override;

	// �� ��ƼŬ �Ӽ��� �����մϴ�.	
	virtual ParticleAttribute OnSetAddParticle() override;

	// ��ƼŬ ��輱(IsBounding)�� Ȱ��ȭ �� ���¿���
	// m_boundingBox�� ������ ������ �Ѿ�� ȣ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;
};

END