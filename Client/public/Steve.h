#pragma once

#include "Client_Defines.h"
#include "Pawn.h"
#include "ParticleEventManager.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CSkeletalAnimator;
END

class CSteve final : public CPawn
{
public:
	enum ANIM_type
	{
		Swing_FF, Swing_BF, Swing_BA, Swing_FA, Swing_R, Swing_L, Attack ,INIT,
		Run_FF, Run_BF, Run_BA, Run_FA,
		Wepon_Near_Attack_Pelvis, Wepon_Near_Attack_ArmR, Wepon_Near_Attack_ArmL, Wepon_Near_Attack_Wepon,
		Wepon_Far_Attack_Pelvis, Wepon_Far_Attack_ArmR, Wepon_Far_Attack_ArmL, Wepon_Far_Attack_Wepon
	};

private:
	CSteve(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSteve(const CSteve& Prototype);
	virtual ~CSteve() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	void SetPos(_float3 v3);
	_float3 GetPos();
	void Set_AttackContinue(bool _bool) { m_isAttackContinue = _bool; };
	bool Get_AttackContinue() { return m_isAttackContinue; };
	void Knock_back(const _float3& vforce) override;
	const _float4x4& GetSoketMatrix(int index);

	virtual void Add_Hp(_float fAmount) override;
	virtual void Dead_Pawn() override;

private:
	_float   m_fMouseSensor = { 0.03f };
	ANIM m_eCurAnim = { ANIM_END };
	ANIM m_eRreAnim = { ANIM_END };

	bool isAttack = { false };	
	bool m_IsDashCoolTime = {false};
	float m_fCurrentDashTime = { 0.f };
	float m_fCoolTimeDash = { 0.4f };
	bool m_isAttackContinue = { false };

	bool isFarAttack = { false };

private:
	void	Input_Key(_float fTimeDelta);
	void	Move(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_Bone() override;
	virtual HRESULT Ready_Animation() override;

private:
	void Update_State(_float fTimeDelta) override;
	void Motion_Idle(_float fTimeDelta) override;
	void Motion_Walk(_float fTimeDelta) override;
	void Motion_Run(_float fTimeDelta) ;
	void Motion_Attack(_float fTimeDelta);
	void Motion_Wepon_Attack(_float fTimeDelta);
	void Motion_Wepon_Attack2(_float fTimeDelta);
	void Turn(_float fTimeDelta) override;

	// 달리기 파티클.
	void PlayDashParticle(_float fTimeDelta);

	// 달리기 파티클 초기화.
	void ResetDashParticle();

public:
	static CSteve* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();


	// 여기서 프레임 콜백을 받음
	void FrameCallback(int animType, int frame) override;

};




