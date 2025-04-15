#pragma once
#include "BreakableCube.h"
#include "ParticleEventManager.h"

class CCobbleStone : public CBreakableCube
{
protected:
	CCobbleStone(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCobbleStone(const CCobbleStone& Prototype);
	virtual ~CCobbleStone() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	HRESULT Drop_Item_OnDestroy(const _float3& fPos);
	HRESULT Play_Destroy_Effect(const _float3& fPos);
	//부수는 사운드
	virtual void PlaySound_Hit(_float3 vPos)override;
	virtual void Play_Create_Sound(_float3 vPos)override;

protected:
	HRESULT Ready_Components();
public:
	static CCobbleStone* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
