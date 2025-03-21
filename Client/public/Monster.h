#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Behavior_Tree.h"

BEGIN(Client)

class CMonster abstract : public CGameObject
{
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

protected:
	CNode* m_pBehaviorTree{ nullptr };
	HRESULT Ready_BehaviorTree();

private:
	HRESULT Ready_Components();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END