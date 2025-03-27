#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Behavior_Tree.h"

BEGIN(Client)

class CMonster abstract : public CGameObject
{
public:
	enum ANIM
	{
		IDLE, RUN, WALK, ATTACK, JUMP, ANIM_END
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
	CGameObject*			Get_Target();
	void					Set_Target(CGameObject* pGameObject);
	CTransform*				Get_Transform();

protected:
	CGameObject* m_pTarget{ nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CSkeletalAnimator* m_skelAnime = { nullptr };
	vector<CVIBuffer_Cube*> m_pVIBufferComs;

protected:
	//CNode* m_pBehaviorTree{ nullptr };
	HRESULT Ready_BehaviorTree();

protected:
	virtual HRESULT Ready_Components() = 0;
	virtual HRESULT Ready_Bone() =0;
	virtual HRESULT Ready_Animation() = 0;

protected:
	virtual void Update_State(_float fTimeDelta) = 0;
	virtual void Motion_Idle(_float fTimeDelta) = 0;
	virtual void Motion_Walk(_float fTimeDelta) = 0;
	virtual void Turn(_float fTimeDelta) = 0;


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END