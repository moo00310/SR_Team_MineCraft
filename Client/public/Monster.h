#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Behavior_Tree.h"
#include <iostream>

BEGIN(Client)

class CMonster abstract : public CGameObject
{
public:
	enum ANIM
	{
		IDLE, RUN, WALK, ATTACK, DEAD, ANIM_END
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
	CGameObject* Get_Target() { return m_pTargetGameObject;}
	void Set_Target(CGameObject* object) { m_pTargetGameObject = object;}
	CTransform* Get_Transform(){ return m_pTransformCom; }
	void		Set_Animation(ANIM etype) { m_eCurAnim = etype; }

protected:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CSkeletalAnimator* m_skelAnime = { nullptr };
	vector<CVIBuffer_Cube*> m_pVIBufferComs;

protected:
	CGameObject* m_pTargetGameObject = nullptr;

protected:
	//CNode* m_pBehaviorTree{ nullptr };
	ANIM m_eCurAnim = { ANIM_END };

protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Bone()= 0;
	virtual HRESULT Ready_Animation() = 0;
	HRESULT Ready_BehaviorTree();

protected:
	virtual void Update_State(_float fTimeDelta) = 0;
	virtual void Motion_Idle(_float fTimeDelta) = 0;
	virtual void Motion_Walk(_float fTimeDelta) = 0;
	virtual void Motion_Attack(_float fTimeDelta) = 0;
	virtual void Turn(_float fTimeDelta) = 0;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END