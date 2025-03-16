#pragma once
#include "BreakableCube.h"

class CLeaf : public CBreakableCube
{
protected:
	CLeaf(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLeaf(const CLeaf& Prototype);
	virtual ~CLeaf() = default;

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
	static CLeaf* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};