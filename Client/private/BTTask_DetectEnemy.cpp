#include "BTTask_DetectEnemy.h"
#include "Monster.h"

STATUS CBTTask_DetectEnemy::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);

	if (pMonster->isFind())
	{
		return STATUS::SUCCESS;
	}

	//적을 탐지 하는 방법
	//1. 거리를 계산해서 가까운지 확인
	//2. 자기 머리와, 상대 머리사이 레이를 쏜다. (예정, 티가 안나서)
	//3. 레이를 쏴서 블럭이 탐지되면 탐지 못했음으로 판단.(예정, 티가 안나서)

	_float fDist{ pMonster->Comput_Distance() };

	if (fDist < 10.f)
	{
		pMonster->Set_Find(true);
		return STATUS::SUCCESS;
	}
	else
	{
		return STATUS::FAIL;
	}

	//_float fHitDist{};
	//CGameObject* pHitOjbect{ nullptr };

	//_float3 temp = pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	//temp.y += 1;

	//_float3 vRayStart = pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);


	//pHitOjbect = pMonster->Get_GameInstance()->Ray_Cast(temp,
	//	pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK),
	//	10.f,
	//	COLLISION_PLAYER
	//	//fHitDist
	//	);

	/*if (pHitOjbect)
	{
		pMonster->Set_Find(true);
		return STATUS::SUCCESS;
	}*/


}
