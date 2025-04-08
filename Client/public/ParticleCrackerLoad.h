#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"

BEGIN(Client)

// ���� ��? ���󰡴� ��ƼŬ.
class CParticleCrackerLoad final : public CParticleSystem
{
private:
	CParticleCrackerLoad(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleCrackerLoad(const CParticleCrackerLoad& Prototype);
	virtual ~CParticleCrackerLoad() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;	

	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleCrackerLoad* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
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