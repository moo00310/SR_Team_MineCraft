#pragma once
#include "Client_Defines.h"
#include "Behavior_Tree.h"
//#include "HeroEnemy.h"
#include "Transform.h"
#include "GameInstance.h"
BEGIN(Client)
class CHeroEnemy;


class CCheck_Enemy_Node : public CNode
{
public:
	STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override;

private:
	CHeroEnemy* m_pHeroEnemy{ nullptr };
	CGameInstance* m_pGameInstance{ nullptr };

};
END

