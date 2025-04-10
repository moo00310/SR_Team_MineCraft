#include "BTTask_RunChase.h"
#include "Monster.h"

STATUS CBTTask_RunChase::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);

	if (pMonster->isFind()) return STATUS::SUCCESS;

	if (pMonster->Comput_Distance() <= 3.f)
	{
		pMonster->Set_Find(true);
		return STATUS::SUCCESS;
	}

	pMonster->Chase_Player(_fTimeDelta);
	pMonster->Set_Animation(CMonster::RUN);
	return STATUS::RUNNING;
}
