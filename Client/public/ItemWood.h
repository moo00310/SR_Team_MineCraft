#pragma once
#include "ItemCube.h"
#include "ParticleSystem.h"
class CItemWood : public CItemCube
{
protected:
	CItemWood(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItemWood(const CItemWood& Prototype);
	virtual ~CItemWood() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
protected:
	HRESULT Ready_Components();

public:
	static CItemWood* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
