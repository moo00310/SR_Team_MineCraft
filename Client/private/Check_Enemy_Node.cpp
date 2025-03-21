#include "Check_Enemy_Node.h"
#include "GameInstance.h"
#include "HeroEnemy.h"

STATUS CCheck_Enemy_Node::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CHeroEnemy* pHeroEnemy = static_cast<CHeroEnemy*>(_Obj);

	if (pHeroEnemy->Get_Target())
	{
		return STATUS::SUCCESS;
	}

	_float fHitDist{};
	CGameObject* pHitObject{ nullptr };

	pHitObject = pHeroEnemy->Get_GameInstance()->Ray_Cast(pHeroEnemy->Get_Transform()->Get_State(CTransform::STATE_POSITION),
		pHeroEnemy->Get_Transform()->Get_State(CTransform::STATE_LOOK),
		5.f,
		COLLISION_PLAYER,
		fHitDist
	);

	pHeroEnemy->Set_Target(pHitObject);

	if (pHitObject)
	{
		pHeroEnemy->Set_Target_Transform(static_cast<CTransform*>(pHeroEnemy->Get_Target()->Find_Component(TEXT("Com_Transform"))));

		return STATUS::SUCCESS;
	}
	else
	{
		return STATUS::FAIL;
	}
	
}
