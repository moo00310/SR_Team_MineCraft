#include "Chase_Enemy_Node.h"
#include "GameInstance.h"
#include "HeroEnemy.h"

CChase_Enemy_Node::CChase_Enemy_Node(CHeroEnemy* pEnemy)
	:m_pHeroEnemy(pEnemy)
{
}

CChase_Enemy_Node::~CChase_Enemy_Node()
{
}

STATUS CChase_Enemy_Node::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	if (!m_pHeroEnemy->Get_Target())
		return STATUS::FAIL;

	m_pHeroEnemy ->Get_Transform()->LookAt(m_pHeroEnemy->Get_Target_Transform()->Get_State(CTransform::STATE_POSITION));
	m_pHeroEnemy->Get_Transform()->Chase(m_pHeroEnemy->Get_Target_Transform()->Get_State(CTransform::STATE_POSITION), _fTimeDelta, 10.f);

	return STATUS::SUCCESS;
}

CChase_Enemy_Node* CChase_Enemy_Node::Create(CHeroEnemy* pEnemy)
{
	return new CChase_Enemy_Node(pEnemy);
}
