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

	pMonster->Chase_Player(_fTimeDelta);


	return STATUS::SUCCESS;
}
