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
	HRESULT Drop_Item_OnDestroy(const _float3& fPos);
	HRESULT Play_Destroy_Effect(const _float3& fPos);
protected:
	HRESULT Ready_Components();
	

public:
	static CCoalOre* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
