#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "Behavior_Tree.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CSkeletalAnimator;
END

class CWarden : public CMonster
{
public:
	enum ANIM_type
	{
		Swing_Pevis, Swing_Leg_R, Swing_Leg_L, Swing_Arm_R, Swing_Arm_L
		, Swing_L, Attack, Dead, INIT,
	};

private:
	CWarden(LPDIRECT3DDEVICE9 pGraphic_Device);
	CWarden(const CWarden& Prototype);
	virtual ~CWarden() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;


private:
	HRESULT Ready_Components();
	HRESULT Ready_Bone() override;
	HRESULT Ready_Animation() override;

private:
	void Update_State(_float fTimeDelta) override;
	void Motion_Idle(_float fTimeDelta) override;
	void Motion_Walk(_float fTimeDelta) override;
	void Motion_Attack(_float fTimeDelta) override;
	void Motion_Dead(_float fTimeDelta) override;
	void Turn(_float fTimeDelta) override;

private:
	virtual HRESULT Ready_BehaviorTree() override;
	void FrameCallback(int animType, int frame) override;

public:
	static CWarden* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	
	virtual void Free();

};

