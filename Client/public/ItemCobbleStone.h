#pragma once
#include "ItemCube.h"
#include "ParticleSystem.h"
class CItemCobbleStone : public CItemCube
{
protected:
	CItemCobbleStone(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItemCobbleStone(const CItemCobbleStone& Prototype);
	virtual ~CItemCobbleStone() = default;

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
	static CItemCobbleStone* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
