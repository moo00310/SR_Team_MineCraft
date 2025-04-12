#include "BTDecorator_IsCutScene.h"
#include "Monster.h"

STATUS CBTDecorator_IsCutScene::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);
	m_fTime += _fTimeDelta;

	if (m_fTime >= 5.f)
	{
		return STATUS::FAIL;
	}

	if(pMonster->GetAnimEnd(CMonster::IDLE))
	{
		TrueNode->Excute(_Obj, _fTimeDelta);
		return STATUS::RUNNING;
	}
	else
	{
		pMonster->Set_Animation(CMonster::IDLE);
		return STATUS::RUNNING;
	}
}
