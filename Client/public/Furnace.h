#pragma once
#include "BreakableCube.h"
class CFurnace : public CBreakableCube
{
protected:
	CFurnace(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFurnace(const CFurnace& Prototype);
	virtual ~CFurnace() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	HRESULT Drop_Item_OnDestroy(const _float3& fPos);
	HRESULT Play_Destroy_Effect(const _float3& fPos);

	bool Get_FurnaceBurn() { return m_bFurnaceBurn; }
	int Get_CoalTime() { return m_iCoalTime; }
	int Get_IronTime() { return m_iIronTime; }

protected:
	HRESULT Ready_Components();
private:
	CTexture* m_pTextureCom2 = nullptr;
	bool m_bFurnaceBurn = false;

	int m_iCoalTime = 0;
	int m_iIronTime = 0;

public:
	static CFurnace* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
