#pragma once
#include "BaseParticleDestroy.h"

BEGIN(Client)

class CParticleLeafDestroy final : public CBaseParticleDestroy
{
private:
	CParticleLeafDestroy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CParticleLeafDestroy(const CParticleLeafDestroy& Prototype);
	virtual ~CParticleLeafDestroy() = default;

public:
	// CParticleSystem��(��) ���� ��ӵ�
	CGameObject* Clone(void* pArg) override;
	static CParticleLeafDestroy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void OnPushPool() override;

protected:
	virtual HRESULT Ready_Components() override;
};

END