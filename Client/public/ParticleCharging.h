#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"

BEGIN(Client)

class CParticleCharging : public CParticleSystem
{
private:
	CParticleCharging(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleCharging(const CParticleCharging& Prototype);
	virtual ~CParticleCharging() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleCharging* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();

protected:
	virtual HRESULT Ready_Components() override;

	// �� ��ƼŬ �Ӽ��� �����մϴ�.	
	virtual ParticleAttribute OnSetAddParticle() override;

	// ��ƼŬ ��輱(IsBounding)�� Ȱ��ȭ �� ���¿���
	// m_boundingBox�� ������ ������ �Ѿ�� ȣ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;
};

END