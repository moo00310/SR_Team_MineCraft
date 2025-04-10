#pragma once

#include "Client_Defines.h"
#include "Behavior_Tree.h"
#include "GameInstance.h"

BEGIN(Client)

class CBTTask_RunChase : public CNode
{
public:
    STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override;
};
END