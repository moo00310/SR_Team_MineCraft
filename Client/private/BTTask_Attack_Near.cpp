#include "BTTask_Attack_Near.h"
#include "Monster.h"

STATUS CBTTask_Attack_Near::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	cout << "±Ù°Å¸®" << endl;
	CMonster* pMonster = static_cast<CMonster*>(_Obj);

	if (pMonster->GetAnimEnd(CMonster::ATTACK))
	{
		pMonster->Set_isAttack(true);
		return STATUS::SUCCESS;
	}

	pMonster->Set_Animation(CMonster::ATTACK);
	return STATUS::RUNNING;
}
