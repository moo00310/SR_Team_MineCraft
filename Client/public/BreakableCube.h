#pragma once
// 부숴지는 큐브들의 부모가 되는 곳

#include "Client_Defines.h"
#include "Cube.h"

class CBreakableCube : public CCube
{
protected:
	CBreakableCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBreakableCube(const CBreakableCube& Prototype);
	virtual ~CBreakableCube() = default;

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
	static CBreakableCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

