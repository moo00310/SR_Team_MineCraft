#include "BTTask_Attack.h"
#include "Monster.h"

STATUS CBTTask_Attack::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	// ���� �ִϸ��̼�
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	pMonster->Set_Animation(CMonster::ATTACK2);
	//pMonster->LookAtPlayer(_fTimeDelta);
	return STATUS::RUNNING;
}
