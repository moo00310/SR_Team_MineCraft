#pragma once
#include "Client_Defines.h"
#include "ParticleSystem.h"

BEGIN(Client)

// ���� ��ƼŬ.
class CParticleDie final : public CParticleSystem
{
private:
	CParticleDie(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleDie(const CParticleDie& Prototype);
	virtual ~CParticleDie() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleDie* Create(LPDIRECT3DDEVICE9 pGraphic_Device);

protected:
	virtual HRESULT Ready_Components() override;

	// �� ��ƼŬ �Ӽ��� �����մϴ�.	
	virtual ParticleAttribute OnSetAddParticle() override;

	// ��ƼŬ ��輱(IsBounding)�� Ȱ��ȭ �� ���¿���
	// m_boundingBox�� ������ ������ �Ѿ�� ȣ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;
};

END