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
    _float  m_fTargetRotation{ 0.0f }; // ��ǥ ȸ�� ����
    _bool   m_isTurning{ false };
    _float  m_fTurnElapsedTime{ 0.0f }; // ȸ�� ���� �ð�
    _float  m_fTurnDuration{ 0.0f };    // ȸ�� ���� �ð�
};
END

