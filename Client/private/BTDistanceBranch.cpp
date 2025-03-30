#include "BTDistanceBranch.h"
#include "Monster.h"

STATUS CBTDistanceBranch::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
    CMonster* pMonster = static_cast<CMonster*>(_Obj);
    if (!pMonster->Get_Target()) return STATUS::FAIL;

    // 몬스터와 타겟의 위치를 얻음
    _float3 vMonsterPos = pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);
    _float3 vTargetPos = static_cast<CTransform*>(pMonster->Get_Target()->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

    // 두 위치의 차이를 계산
    _float3 vDiff = vTargetPos - vMonsterPos;

    // 거리를 계산
    float fDistance = D3DXVec3Length(&vDiff);

    if (fDistance > 0.01f)
    {
        if (fDistance <= m_fAttack_Distnace)
        {
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
            if (fDistance <= 10.f)
            {
                return m_pChaseAction ? m_pChaseAction->Excute(_Obj, _fTimeDelta) : STATUS::FAIL;
            }
        }
    }

    pMonster->Set_Find(false);
    return STATUS::FAIL;
}
