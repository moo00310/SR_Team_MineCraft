#include "Check_Enemy_Node.h"
#include "GameInstance.h"
#include "HeroEnemy.h"

STATUS CCheck_Enemy_Node::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	//부모에서 게임인스턴스를 받아버리자
	if (!m_pGameInstance)
	{
		m_pGameInstance = CGameInstance::Get_Instance();
	}

	if (!m_pHeroEnemy)
	{
		m_pHeroEnemy = dynamic_cast<CHeroEnemy*>(_Obj);
	}

	_bool isHit{ false };
	_float fHitDist;

	isHit = m_pGameInstance->Ray_Cast(m_pHeroEnemy->m_pTransformCom->Get_State(CTransform::STATE_POSITION),
		m_pHeroEnemy->m_pTransformCom->Get_State(CTransform::STATE_LOOK),
		5.f,
		COLLISION_PLAYER,
		fHitDist,
		&m_pHeroEnemy->m_pTarget);

	if (isHit)
	{
		m_pHeroEnemy->m_pTargetTransform = static_cast<CTransform*>(m_pHeroEnemy->m_pTarget->Find_Component(TEXT("Com_Transform")));

		return STATUS::SUCCESS;
	}
	else
	{
		return STATUS::FAIL;
	}
	
}
