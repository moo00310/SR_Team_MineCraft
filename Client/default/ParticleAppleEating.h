#pragma once
#include "BaseParticleEating.h"

// ��� �Դ� ��ƼŬ.
class CParticleAppleEating : public CBaseParticleEating
{
private:
	CParticleAppleEating(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleAppleEating(const CParticleAppleEating& Prototype);
	virtual ~CParticleAppleEating() = default;

public:
	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleAppleEating* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();

protected:
	// �� ��ƼŬ �Ӽ��� �����մϴ�.	
	virtual ParticleAttribute OnSetAddParticle() override;

	// ��ƼŬ ��輱(IsBounding)�� Ȱ��ȭ �� ���¿���
	// m_boundingBox�� ������ ������ �Ѿ�� ȣ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;
};