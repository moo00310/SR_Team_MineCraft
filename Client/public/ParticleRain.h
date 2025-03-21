#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"

BEGIN(Engine)

// �� ��ƼŬ.
class CParticleRain final : public CParticleSystem
{
private:
	CParticleRain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleRain(const CParticleRain& Prototype);
	virtual ~CParticleRain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;		

	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleRain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();

protected:
	virtual HRESULT Ready_Components();

	// �� ��ƼŬ �Ӽ��� �����մϴ�.
	virtual ParticleAttribute OnSetAddParticle() override;
	
	// ��ƼŬ ��輱(IsBounding)�� Ȱ��ȭ �� ���¿���
	// m_boundingBox�� ������ ������ �Ѿ�� ȣ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;
};

END