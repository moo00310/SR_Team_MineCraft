#include "BTTask_Find.h"
#include "Monster.h"

STATUS CBTTask_Find::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	pMonster->Set_Animation(CMonster::FIND);
	return STATUS::RUNNING;
}
