#include "BTDecorator_IsTargetNear.h"
#include "Monster.h"

STATUS CBTDecorator_IsTargetNear::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
    CMonster* pMonster = static_cast<CMonster*>(_Obj);

    float fDistance = pMonster->Comput_Distance();

   if (fDistance <= 10.0f && fDistance > 0.01f)
   {
       TrueNode->Excute(_Obj, _fTimeDelta);
       return STATUS::RUNNING;
   }
   else
   {
       pMonster->Set_Find(false);
       return STATUS::FAIL;
   }
}
