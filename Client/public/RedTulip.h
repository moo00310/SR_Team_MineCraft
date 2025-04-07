#pragma once
#include "BreakableRect.h"
class CRedTulip : public CBreakableRect
{
protected:
	CRedTulip(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRedTulip(const CRedTulip& Prototype);
	virtual ~CRedTulip() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	HRESULT Drop_Item_OnDestroy(const _float3& fPos)override;

protected:
	HRESULT Ready_Components();

public:
	static CRedTulip* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
