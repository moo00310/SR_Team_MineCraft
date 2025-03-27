#pragma once
#include "BreakableCube.h"
class CStone : public CBreakableCube
{
protected:
	CStone(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStone(const CStone& Prototype);
	virtual ~CStone() = default;

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
	static CStone* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
