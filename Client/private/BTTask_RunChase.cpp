#include "BTTask_RunChase.h"
#include "Monster.h"

STATUS CBTTask_RunChase::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	pMonster->Set_Animation(CMonster::RUN);
	return STATUS::SUCCESS;
}
