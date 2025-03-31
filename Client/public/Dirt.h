#pragma once
#include "BreakableCube.h"
#include "ParticleEventManager.h"

class CDirt : public CBreakableCube
{
protected:
	CDirt(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDirt(const CDirt& Prototype);
	virtual ~CDirt() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	HRESULT Delete_Cube(_float3 fPos) override;

protected:
	HRESULT Ready_Components();
public:
	static CDirt* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
