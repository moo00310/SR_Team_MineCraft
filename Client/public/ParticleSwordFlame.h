#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"

BEGIN(Client)

class CParticleSwordFlame : public CParticleSystem
{
private:
	CParticleSwordFlame(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleSwordFlame(const CParticleSwordFlame& Prototype);
	virtual ~CParticleSwordFlame() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleSwordFlame* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
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