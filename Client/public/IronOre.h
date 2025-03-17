#pragma once
#include "BreakableCube.h"
class CIronOre : public CBreakableCube
{
protected:
	CIronOre(LPDIRECT3DDEVICE9 pGraphic_Device);
	CIronOre(const CIronOre& Prototype);
	virtual ~CIronOre() = default;

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
	static CIronOre* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
