#pragma once
#include "BreakableCube.h"
class CCoalOre : public CBreakableCube
{
protected:
	CCoalOre(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCoalOre(const CCoalOre& Prototype);
	virtual ~CCoalOre() = default;

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
	static CCoalOre* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
