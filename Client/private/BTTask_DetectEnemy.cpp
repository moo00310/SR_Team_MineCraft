#include "BTTask_DetectEnemy.h"
#include "Monster.h"

STATUS CBTTask_DetectEnemy::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);

	if (pMonster->Get_Target())
	{
		return STATUS::SUCCESS;
	}

	_bool isHit{ false };
	_float fHitDist;

	isHit = pMonster->Get_GameInstance()->Ray_Cast(pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION),
		pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK),
		10.f,
		COLLISION_PLAYER,
		fHitDist,
		&pMonster->Get_Target());

	if (isHit)
	{
		return STATUS::SUCCESS;
	}
	else
	{
		return STATUS::FAIL;
	}
}
