#include "BTTask_Find.h"
#include "Monster.h"

STATUS CBTTask_Find::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);

	if (pMonster->GetAnimEnd(CMonster::FIND))
	{
		return STATUS::SUCCESS;
	}

	cout << "Æ÷È¿" << endl;
	pMonster->Set_Animation(CMonster::FIND);
	return STATUS::RUNNING;
}
