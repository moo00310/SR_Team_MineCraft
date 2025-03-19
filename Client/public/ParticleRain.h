#pragma once
#include "ParticleSystem.h"
#include "Client_Defines.h"

// �� ��ƼŬ.
class CParticleRain : public CParticleSystem
{
private:
	CParticleRain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleRain(const CParticleRain& Prototype);
	virtual ~CParticleRain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleRain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();

protected:
	virtual HRESULT Ready_Components();

	// �� ��ƼŬ �Ӽ��� �����մϴ�.
	// �θ� Initialize ȣ�� �� �ݹ�.
	virtual ParticleAttribute OnSetAddParticle() override;
	
	// ��ƼŬ ��輱(IsBounding)�� Ȱ��ȭ �� ���¿���
	// m_boundingBox�� ������ ������ �Ѿ�� ȣ��.
	virtual void OnBoundingExit(ParticleAttribute& particle) override;

private:
	HRESULT PrevRender() override;
	HRESULT EndRender() override;
};

