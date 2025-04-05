#include "BTTask_Chase.h"
#include "Monster.h"

STATUS CBTTask_Chase::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);


	if (!pMonster->isFind())
	{
		return STATUS::FAIL;
	}

	pMonster->Chase_Player(_fTimeDelta);
	pMonster->Set_Animation(CMonster::WALK);


	return STATUS::SUCCESS;
}
