#include "Check_Enemy_Node.h"
#include "GameInstance.h"
#include "HeroEnemy.h"

CCheck_Enemy_Node::CCheck_Enemy_Node(CHeroEnemy* pEnemy)
	:m_pHeroEnemy(pEnemy)
{
}

CCheck_Enemy_Node::~CCheck_Enemy_Node()
{
}

STATUS CCheck_Enemy_Node::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	if (!m_pHeroEnemy)
	{
		m_pHeroEnemy = dynamic_cast<CHeroEnemy*>(_Obj);
	}

	if (m_pHeroEnemy->Get_Target())
	{
		return STATUS::SUCCESS;
	}

	_bool isHit{ false };
	_float fHitDist;

	isHit = m_pHeroEnemy->Get_GameInstance()->Ray_Cast(m_pHeroEnemy->Get_Transform()->Get_State(CTransform::STATE_POSITION),
		m_pHeroEnemy->Get_Transform()->Get_State(CTransform::STATE_LOOK),
		5.f,
		COLLISION_PLAYER,
		fHitDist,
		&m_pHeroEnemy->Get_Target());

	if (isHit)
	{
 		m_pHeroEnemy->Set_Target_Transform(static_cast<CTransform*>(m_pHeroEnemy->Get_Target()->Find_Component(TEXT("Com_Transform"))));

		return STATUS::SUCCESS;
	}
	else
	{
		return STATUS::FAIL;
	}
	
}

CCheck_Enemy_Node* CCheck_Enemy_Node::Create(CHeroEnemy* pEnemy)
{
	return new CCheck_Enemy_Node(pEnemy);
}
