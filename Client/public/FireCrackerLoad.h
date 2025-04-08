#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
#include "ParticleEventManager.h"

BEGIN(Client)

// ÆøÁ× ±æ.
class CFireCrackerLoad final : public CGameObject
{
private:
	CFireCrackerLoad(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFireCrackerLoad(const CFireCrackerLoad& Prototype);
	virtual ~CFireCrackerLoad() = default;

public:
	static CFireCrackerLoad* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	virtual void OnPushPool() override;

	CTransform* GetTransform() const;

private:
	HRESULT Ready_Components();

private:
	CTransform* m_pTransformCom = { nullptr };
	_float fEndCount = { 0.5f };
	_float fCurrentCount = { 0.f };
};

END