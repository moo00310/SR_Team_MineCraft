#include "BTDecorator_IsTargetNear.h"
#include "Monster.h"

STATUS CBTDecorator_IsTargetNear::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
    CMonster* pMonster = static_cast<CMonster*>(_Obj);

    // 몬스터와 타겟의 위치를 얻음
    _float3 vMonsterPos = pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);
    _float3 vTargetPos = static_cast<CTransform*>(pMonster->Get_Target()->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

    // 두 위치의 차이를 계산
    _float3 vDiff = vTargetPos - vMonsterPos;

    // 거리를 계산
    float fDistance = D3DXVec3Length(&vDiff);

    if (fDistance <= 10.0f && fDistance > 0.01f)
    {
        TrueNode->Excute(_Obj, _fTimeDelta);
        return STATUS::RUNNING;
    }
    else
    {
        pMonster->Set_Target(nullptr);
        return STATUS::FAIL;
    }
}
