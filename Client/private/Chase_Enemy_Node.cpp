#include "Chase_Enemy_Node.h"
#include "GameInstance.h"
#include "HeroEnemy.h"

STATUS CChase_Enemy_Node::Excute(CGameObject* _Obj, _float _fTimeDelta)
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

	m_pHeroEnemy ->m_pTransformCom->LookAt(m_pHeroEnemy -> m_pTargetTransform->Get_State(CTransform::STATE_POSITION));
	m_pHeroEnemy->m_pTransformCom->Chase(m_pHeroEnemy->m_pTargetTransform->Get_State(CTransform::STATE_POSITION), _fTimeDelta, 10.f);

	return STATUS::SUCCESS;
}
