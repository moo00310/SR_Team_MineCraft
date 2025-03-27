#include "BTTask_Attack.h"
#include "Monster.h"

STATUS CBTTask_Attack::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	// 공격 애니메이션
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	pMonster->Set_Animation(CMonster::ATTACK);
	cout << "공격" << endl;
	fTime += _fTimeDelta;

	if (fTime > 3.f)
	{
		return STATUS::SUCCESS;
	}

	return STATUS::RUNNING;
}
