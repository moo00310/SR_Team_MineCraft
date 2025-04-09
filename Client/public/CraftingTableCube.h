#pragma once
#include "BreakableCube.h"
class CCraftingTableCube : public CBreakableCube
{
protected:
	CCraftingTableCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCraftingTableCube(const CCraftingTableCube& Prototype);
	virtual ~CCraftingTableCube() = default;

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
	static CCraftingTableCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
