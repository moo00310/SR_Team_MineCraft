#include "BTTask_Chase.h"
#include "Monster.h"

STATUS CBTTask_Chase::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	pMonster->Set_Animation(CMonster::WALK);

	if (!pMonster->Get_Target())
	{
		return STATUS::FAIL;
	}

	CTransform* pTargetTransform{ static_cast<CTransform*> (pMonster->Get_Target()->Find_Component(TEXT("Com_Transform"))) };

	_float3 vector = pTargetTransform->Get_State(CTransform::STATE_POSITION);
	pMonster->Get_Transform()->LookAt_XZ(vector);
	pMonster->Get_Transform()->Chase(_float3(vector.x, 0.f, vector.z), _fTimeDelta, 1.0f);

	return STATUS::SUCCESS;
}
