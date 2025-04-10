#include "BTTask_Idle.h"
#include "Monster.h"

STATUS CBTTask_Idle::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	pMonster->Set_IDLETimeOver(_fTimeDelta);

	if (pMonster->Get_IDLETimeOver() >= 3.f)
	{
		pMonster->Init_BlackBoard();
		return STATUS::SUCCESS;
	}

	pMonster->Set_Animation(CMonster::IDLE);
	return STATUS::RUNNING;
}
