#include "Chase_Enemy_Node.h"
#include "GameInstance.h"
#include "HeroEnemy.h"

STATUS CChase_Enemy_Node::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	m_pHeroEnemy ->Get_Transform()->LookAt(m_pHeroEnemy->Get_Target_Transform()->Get_State(CTransform::STATE_POSITION));
	m_pHeroEnemy->Get_Transform()->Chase(m_pHeroEnemy->Get_Target_Transform()->Get_State(CTransform::STATE_POSITION), _fTimeDelta, 10.f);

	return STATUS::SUCCESS;
}
