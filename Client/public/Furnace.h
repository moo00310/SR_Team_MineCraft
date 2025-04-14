#pragma once
#include "BreakableCube.h"
#include "ParticleEventManager.h"

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
	float Get_CoalTime() { return m_fCoalTime; }
	float Get_IronTime() { return m_fIronTime; }	
	const float GetCoalCoolTime() const { return m_fCoalCoolTime; }
	const float GetIronCoolTime() const { return m_fIronCoolTime; }

protected:
	HRESULT Ready_Components();
private:
	CTexture* m_pTextureCom2 = nullptr;
	bool m_bFurnaceBurn = false;
	bool m_bParticle = false;

	float m_fCoalTime = 0.f;
	float m_fIronTime = 0.f;

	const float const m_fCoalCoolTime = 150.f;		// 석탄 하나 당 화로 유지 시간.
	const float const m_fIronCoolTime = 18.f;		// 철이 나오는 시간.

public:
	static CFurnace* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
