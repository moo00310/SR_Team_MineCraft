#include "BTTask_DetectEnemy.h"
#include "Monster.h"

STATUS CBTTask_DetectEnemy::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);

	if (pMonster->isFind())
	{
		return STATUS::SUCCESS;
	}

	//���� Ž�� �ϴ� ���
	//1. �Ÿ��� ����ؼ� ������� Ȯ��
	//2. �ڱ� �Ӹ���, ��� �Ӹ����� ���̸� ���. (����, Ƽ�� �ȳ���)
	//3. ���̸� ���� ���� Ž���Ǹ� Ž�� ���������� �Ǵ�.(����, Ƽ�� �ȳ���)

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
