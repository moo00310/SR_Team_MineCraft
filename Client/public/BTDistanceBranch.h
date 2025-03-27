#pragma once

#include "Client_Defines.h"
#include "Behavior_Tree.h"
#include "GameInstance.h"

BEGIN(Client)

class CBTDistanceBranch : public CNode
{
public:
    void Set_Actions(CNode* pAttack, CNode* pChase) {
        m_pAttackAction = pAttack;
        m_pChaseAction = pChase;
    }

    virtual STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override;

    virtual void Free() override
    {
        __super::Free(); 
        Safe_Release(m_pAttackAction);
        Safe_Release(m_pChaseAction);
    }
private:
    CNode* m_pAttackAction = nullptr;
    CNode* m_pChaseAction = nullptr;
};

END
