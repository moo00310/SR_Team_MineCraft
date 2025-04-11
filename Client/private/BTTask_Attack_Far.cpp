#include "BTTask_Attack_Far.h"
#include "Monster.h"

STATUS CBTTask_Attack_Far::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	if (pMonster->GetAnimEnd(CMonster::ATTACK2))
	{
		pMonster->Set_isAttack(true);
		return STATUS::SUCCESS;
	}

	pMonster->Set_Animation(CMonster::ATTACK2);
	return STATUS::RUNNING;
}
