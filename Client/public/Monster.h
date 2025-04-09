#pragma once
#include "Client_Defines.h"
#include "Pawn.h"
#include "Behavior_Tree.h"
#include <iostream>

BEGIN(Client)

class CMonster abstract : public CPawn
{
public:
	enum MonsterType
	{
		MT_Zombie, MT_Creeper, MT_WARDEN, MT_END
	};

protected:
	CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	CPawn* Get_Target() { return m_pTargetPawn; }
	void  Set_Animation(ANIM etype) { m_eCurAnim = etype; }
	float Comput_Distance();
	void Knock_back(const _float3& vforce) override;

	//BT용 겟셋 함수
public:
	_bool isFind() { return m_isFind; }
	void Set_Find(bool flag) { m_isFind = flag; }
	void Chase_Player(float _fTimeDelta);
	void LookAtPlayer(float _fTimeDelta);
	void Init_BlackBoard();
	bool GetAnimEnd(int type);

public:
	_float Get_ChaseTimeOver() { return m_ChaseTime; }
	void Set_ChaseTimeOver(float _fTime) { m_ChaseTime += _fTime; }

	_float Get_IDLETimeOver() { return m_IdleTime; }
	void Set_IDLETimeOver(float _fTime) { m_IdleTime += _fTime; }

	_float Get_isAttack() { return isAttack; }
	void Set_isAttack(bool _b) { isAttack += _b; }
protected:
	CPawn* m_pTargetPawn = nullptr;

protected:
	CNode* m_pBehaviorTree{ nullptr };
	MonsterType m_MonsterType = { MT_END };
	float m_fAttackDistance = {};
	_float3 m_TargetPos = {};

// Black_Board
protected:
	_bool m_isFind = { false };
	_float m_ChaseTime = { 0.f };
	_float m_IdleTime = { 0.f };
	_bool isAttack = { false };

protected:
	bool m_bAnimEnd[ANIM_END];


protected:
	virtual HRESULT Ready_BehaviorTree();

protected:
	HRESULT Ready_Components();
	virtual HRESULT Ready_Bone()= 0;
	virtual HRESULT Ready_Animation() = 0;

protected:
	virtual void Update_State(_float fTimeDelta) = 0;
	virtual void Motion_Idle(_float fTimeDelta) = 0;
	virtual void Motion_Walk(_float fTimeDelta) = 0;
	virtual void Motion_Attack(_float fTimeDelta) = 0;
	virtual void Motion_Dead(_float fTimeDelta) = 0;
	virtual void Turn(_float fTimeDelta) = 0;

protected:
	void OnPushPool() override;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();

	// 콜백 순수 가상함수
	void FrameCallback(int animType, int frame) =0;

};

END