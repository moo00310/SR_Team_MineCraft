#pragma once
#include "Behavior_Tree.h"

BEGIN(Client)

class CBTDecorator_CheckElapsedTime : public CDecorator_If
{
public:
	STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override;

	_float m_fElapsedTime = {};
};

END

