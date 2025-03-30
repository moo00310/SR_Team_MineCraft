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
		MT_Zombie, MT_Creeper, MT_END
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
	CPawn* Get_Target() { return m_pTargetPawn;}
	void		Set_Animation(ANIM etype) { m_eCurAnim = etype; }
	float Comput_Distance();

//BT¿ë °Ù¼Â ÇÔ¼ö
public:
	_bool isFind() { return m_isFind; }
	void Set_Find(bool flag) { m_isFind = flag; }
	void Chase_Player(float _fTimeDelta);

protected:
	CPawn* m_pTargetPawn = nullptr;

protected:
	CNode* m_pBehaviorTree{ nullptr };
	MonsterType m_MonsterType = { MT_END };
	float m_fAttackDistance = {};

// Black_Board
protected:
	_bool m_isFind = { false };

protected:


protected:
	HRESULT Ready_BehaviorTree();

protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Bone()= 0;
	virtual HRESULT Ready_Animation() = 0;

protected:
	virtual void Update_State(_float fTimeDelta) = 0;
	virtual void Motion_Idle(_float fTimeDelta) = 0;
	virtual void Motion_Walk(_float fTimeDelta) = 0;
	virtual void Motion_Attack(_float fTimeDelta) = 0;
	virtual void Motion_Dead(_float fTimeDelta) = 0;
	virtual void Turn(_float fTimeDelta) = 0;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END