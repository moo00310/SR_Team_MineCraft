#include "BTTask_Attack_Far.h"
#include "Monster.h"

STATUS CBTTask_Attack_Far::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	// ���� �ִϸ��̼�
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	pMonster->Set_Animation(CMonster::ATTACK2);
	return STATUS::RUNNING;
}
