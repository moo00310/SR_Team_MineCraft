#pragma once
#include "ItemCube.h"
#include "ParticleSystem.h"
class CItemIronOre : public CItemCube
{
protected:
	CItemIronOre(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItemIronOre(const CItemIronOre& Prototype);
	virtual ~CItemIronOre() = default;

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
	static CItemIronOre* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
