#pragma once

#include "Client_Defines.h"
#include "Behavior_Tree.h"
#include "GameInstance.h"

BEGIN(Client)

class CBTDecorator_IsCutScene : public CDecorator_If
{
public:
	STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override;

	float m_fTime = {};
};
END
