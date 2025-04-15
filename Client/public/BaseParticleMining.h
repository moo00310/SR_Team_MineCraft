#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"
#include "Sun.h"

BEGIN(Client)

// �� ä�� �θ� ��ƼŬ.
class CBaseParticleMining abstract : public CParticleSystem
{
protected:
	CBaseParticleMining(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBaseParticleMining(const CBaseParticleMining& Prototype);
	virtual ~CBaseParticleMining() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	// CParticleSystem��(��) ���� ��ӵ�	
	virtual void Free();

	virtual void OnPushPool() override;

protected:
	virtual HRESULT Ready_Components() = 0;

	// �� ��ƼŬ �Ӽ��� �����մϴ�.
	virtual ParticleAttribute OnSetAddParticle() override;

	// ��ƼŬ ��輱(IsBounding)�� Ȱ��ȭ �� ���¿���
	// m_boundingBox�� ������ ������ �Ѿ�� ȣ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;

	virtual void OnReplay(ParticleAttribute& particle) override;

private:
	CSun* m_Sun = { nullptr };
};

END