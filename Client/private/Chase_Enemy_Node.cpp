#include "Chase_Enemy_Node.h"
#include "GameInstance.h"
#include "HeroEnemy.h"

STATUS CChase_Enemy_Node::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CHeroEnemy* pHeroEnemy = static_cast<CHeroEnemy*>(_Obj);

	if (!pHeroEnemy->Get_Target())
	{
		return STATUS::FAIL;
	}

	pHeroEnemy->Get_Transform()->LookAt(pHeroEnemy->Get_Target_Transform()->Get_State(CTransform::STATE_POSITION));
	pHeroEnemy->Get_Transform()->Chase(pHeroEnemy->Get_Target_Transform()->Get_State(CTransform::STATE_POSITION), _fTimeDelta, 10.f);

	return STATUS::SUCCESS;
}
