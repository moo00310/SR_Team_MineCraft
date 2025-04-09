#include "BTTask_RunChase.h"
#include "Monster.h"

STATUS CBTTask_RunChase::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	pMonster->Set_Animation(CMonster::RUN);
	return STATUS::RUNNING;
}


/// 일정거리 만큼 추적한 다음 SUSSECE를 반환