#include "BTDecorator_IsTargetNear.h"
#include "Monster.h"

STATUS CBTDecorator_IsTargetNear::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
    CMonster* pMonster = static_cast<CMonster*>(_Obj);

    // ���Ϳ� Ÿ���� ��ġ�� ����
    _float3 vMonsterPos = pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);
    _float3 vTargetPos = static_cast<CTransform*>(pMonster->Get_Target()->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);

    // �� ��ġ�� ���̸� ���
    _float3 vDiff = vTargetPos - vMonsterPos;

    // �Ÿ��� ���
    float fDistance = D3DXVec3Length(&vDiff);

    // ���� �Ÿ� ���� �ִ��� Ȯ�� (��: 500.0f)
    if (fDistance <= 20.0f)
    {
        TrueNode->Excute(_Obj, _fTimeDelta);
    }
    else
    {
        pMonster->Set_Target(nullptr);
        return STATUS::FAIL;
    }
}
