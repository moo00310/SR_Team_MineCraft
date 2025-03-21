#include "BTTask_Chase.h"
#include "Monster.h"

STATUS CBTTask_Chase::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);

	if (!pMonster->Get_Target())
	{
		return STATUS::FAIL;
	}

	CTransform* pTargetTransform{ static_cast<CTransform*> (pMonster->Get_Target()->Find_Component(TEXT("Com_Transform"))) };

	pMonster->Get_Transform()->LookAt(pTargetTransform->Get_State(CTransform::STATE_POSITION));
	pMonster->Get_Transform()->Chase(pTargetTransform->Get_State(CTransform::STATE_POSITION), _fTimeDelta, 3.f);

	return STATUS::SUCCESS;
}
