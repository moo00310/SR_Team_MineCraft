#include "BTDecorator_IsAttack.h"
#include "Monster.h"

STATUS CBTDecorator_IsAttack::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
    CMonster* pMonster = static_cast<CMonster*>(_Obj);

    if (!pMonster->Get_isAttack())
    {
        TrueNode->Excute(_Obj, _fTimeDelta);
        return STATUS::RUNNING;
    }
    else
    {
        return STATUS::FAIL;
    }
}

