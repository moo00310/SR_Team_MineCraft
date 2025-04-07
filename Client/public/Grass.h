#pragma once
#include "BreakableRect.h"
class CGrass : public CBreakableRect
{
protected:
	CGrass(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGrass(const CGrass& Prototype);
	virtual ~CGrass() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	HRESULT Drop_Item_OnDestroy(const _float3& fPos)override;
	HRESULT Play_Destroy_Effect(const _float3& fPos)override;
protected:
	HRESULT Ready_Components();

public:
	static CGrass* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
