#pragma once

#include "Client_Defines.h"
#include "RightHand_Object.h"
#include "ParticleEventManager.h"
#include "SwordAura.h"
#include "Camera_Player.h"

class CRect_Model final : public CRightHand_Object
{
protected:
	CRect_Model(LPDIRECT3DDEVICE9 pGraphic_Device);
	CRect_Model(const CRect_Model& Prototype);
	virtual ~CRect_Model() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT Ready_Bone() override;
	HRESULT Ready_Animation() override;

private:
	void Update_State(_float fTimeDelta) override;
	void Motion_Idle(_float fTimeDelta) override;
	void Motion_Swing(_float fTimeDelta) override;
	void Motion_Walk(_float fTimeDelta) override; 
	void Motion_Run(_float fTimeDelta) override; 
	void Motion_EAT(_float fTimeDelta); 
	virtual void KeyInput() override;

private:
	// 불검.
	void FireSword();

	// 검기.
	void AuraSword();

	// 불검 휘두를 때 잔상.
	void SwingFireSword();

	// TODO :: 테스트용 워든 공격 파티클.
	void SonicBoom();

	ITEMNAME Compute_Texture_Name();

	CParticleSystem* flameSword = {nullptr};	

public:
	static CRect_Model* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

private:
	// 먹는거 현재 쿨타임.
	_float m_fCurrentEatTime = {0.f};

	// 먹는거 쿨타임.
	const _float m_fEatCoolTime = {0.23f};
};

