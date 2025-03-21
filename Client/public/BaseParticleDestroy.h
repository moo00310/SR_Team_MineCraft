#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"

BEGIN(Client)

// �� �ı� �� �� �θ� ��ƼŬ.
class CBaseParticleDestroy abstract : public CParticleSystem
{
protected:
	CBaseParticleDestroy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBaseParticleDestroy(const CBaseParticleDestroy& Prototype);
	virtual ~CBaseParticleDestroy() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	// CParticleSystem��(��) ���� ��ӵ�	
	virtual void Free();

protected:
	virtual HRESULT Ready_Components() = 0;

	// �� ��ƼŬ �Ӽ��� �����մϴ�.
	virtual ParticleAttribute OnSetAddParticle() override;

	// ��ƼŬ ��輱(IsBounding)�� Ȱ��ȭ �� ���¿���
	// m_boundingBox�� ������ ������ �Ѿ�� ȣ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;

	virtual void OnReplay(ParticleAttribute& particle) override;
};

END