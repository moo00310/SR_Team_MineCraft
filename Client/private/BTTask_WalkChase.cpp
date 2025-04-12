#include "BTTask_WalkChase.h"
#include "Monster.h"

STATUS CBTTask_WalkChase::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	pMonster->Set_ChaseTimeOver(_fTimeDelta);

	if (pMonster->Get_ChaseTimeOver() >= 3.f)
	{
		return STATUS::FAIL;
	}

	cout << "chase" << endl;
	pMonster->Chase_Player(_fTimeDelta);
	pMonster->Set_Animation(CMonster::WALK);
	return STATUS::SUCCESS;
}
