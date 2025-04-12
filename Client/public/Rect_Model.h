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
	void Motion_Attack1(_float fTimeDelta);
	void Motion_Attack2(_float fTimeDelta);
	virtual void KeyInput() override;

private:
	CParticleSystem* flameSword = { nullptr };
	_float m_fCurrentEatTime = { 0.f }; 	// ¸Ô´Â°Å ÇöÀç ÄðÅ¸ÀÓ.
	const _float m_fEatCoolTime = { 0.23f }; 	// ¸Ô´Â°Å ÄðÅ¸ÀÓ.
	int FX_BoeIndex = {};

private:
	void FireSword(); 	// ºÒ°Ë.
	void AuraSword(); 	// °Ë±â.
	void SwingFireSword();	 	// ºÒ°Ë ÈÖµÎ¸¦ ¶§ ÀÜ»ó.

private:
	ITEMNAME Compute_Texture_Name();
private:
	virtual void FrameCallback(int animType, int frame) override;

public:
	static CRect_Model* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();


};

