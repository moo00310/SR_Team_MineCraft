#pragma once
#include "Behavior_Tree.h"
#include "Client_Defines.h"

BEGIN(Client)

class CHeroEnemy;

class CChase_Enemy_Node : public CNode
{
public:
	STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override;
};
END