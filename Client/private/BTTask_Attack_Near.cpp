#include "BTTask_Attack_Near.h"
#include "Monster.h"

STATUS CBTTask_Attack_Near::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	// ���� �ִϸ��̼�
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	pMonster->Set_Animation(CMonster::ATTACK);
	return STATUS::RUNNING;
}
