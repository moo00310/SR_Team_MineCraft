#include "BTDistanceBranch.h"
#include "Monster.h"

STATUS CBTDistanceBranch::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
    CMonster* pMonster = static_cast<CMonster*>(_Obj);
    if (!pMonster->Get_Target()) return STATUS::FAIL;

    float fDistance = pMonster->Comput_Distance();

    if (fDistance > 0.01f)
    {
        if (fDistance <= m_fAttack_Distnace)
        {
            pMonster->Get_RigidBody()->StopMovement();
            bIsAttack = true;
            return m_pAttackAction ? m_pAttackAction->Excute(_Obj, _fTimeDelta) : STATUS::FAIL;
        }
        else
        {
            if (bIsAttack)
            {
                pMonster->Reset_Ainmation();
                bIsAttack = false;
            }
            if (fDistance <= 15.f)
            {
                return m_pChaseAction ? m_pChaseAction->Excute(_Obj, _fTimeDelta) : STATUS::FAIL;
            }
        }
    }

    pMonster->Set_Find(false);
    return STATUS::FAIL;
}
