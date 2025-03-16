#pragma once
#include "BreakableCube.h"

class CWood : public CBreakableCube
{
protected:
	CWood(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWood(const CWood& Prototype);
	virtual ~CWood() = default;

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
	static CWood* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};