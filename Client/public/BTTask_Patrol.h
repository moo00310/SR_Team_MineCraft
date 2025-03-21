#pragma once

#include "Client_Defines.h"
#include "Behavior_Tree.h"
#include "GameInstance.h"

BEGIN(Client)

class CBTTask_Patrol : public CNode
{
public:
	STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override;

    _float  m_fElapsedTime{ 0.0f };
    _float  m_fTargetRotation{ 0.0f }; // 목표 회전 각도
    _bool   m_isTurning{ false };
    _float  m_fTurnElapsedTime{ 0.0f }; // 회전 진행 시간
    _float  m_fTurnDuration{ 0.0f };    // 회전 지속 시간
};
END

