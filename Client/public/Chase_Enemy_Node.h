#pragma once
#include "Behavior_Tree.h"
#include "Client_Defines.h"

BEGIN(Client)

class CHeroEnemy;

class CChase_Enemy_Node : public CNode
{
private:
	CChase_Enemy_Node() = default;
	CChase_Enemy_Node(CHeroEnemy* pEnemy);
	virtual ~CChase_Enemy_Node();

public:
	STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override;

private:
	CHeroEnemy* m_pHeroEnemy{ nullptr };
public:
	static CChase_Enemy_Node* Create(CHeroEnemy* pEnemy);
};
END