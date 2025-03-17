#pragma once
#include "GameInstance.h"
#include "ParticleSystem.h"
#include "Client_Defines.h"

class CTestParticle final : public CParticleSystem
{
private:
	CTestParticle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTestParticle(const CTestParticle& Prototype);
	virtual ~CTestParticle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;

	// CParticleSystem을(를) 통해 상속됨
	CGameObject* Clone(void* pArg) override;
	static CTestParticle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free();

protected:
	virtual HRESULT Ready_Components();

private:
	HRESULT PrevRender() override;
	HRESULT EndRender() override;

private:
	CTexture* m_pTextureCom = {nullptr};
};

