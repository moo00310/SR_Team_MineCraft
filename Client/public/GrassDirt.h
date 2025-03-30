#pragma once
#include "BreakableCube.h"
#include "ParticleSystem.h"

class CGrassDirt : public CBreakableCube
{
protected:
	CGrassDirt(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGrassDirt(const CGrassDirt& Prototype);
	virtual ~CGrassDirt() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	HRESULT Delete_Cube(_float3 fPos) override;
	HRESULT Create_Cube(_float3 fPos) override;

private:
	// 파괴 파티클 적용.
	void PlayDestroyParticle(_float3 _position);

protected:
	HRESULT Ready_Components();

public:
	static CGrassDirt* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
