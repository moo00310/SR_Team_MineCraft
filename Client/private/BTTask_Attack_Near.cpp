#include "BTTask_Attack_Near.h"
#include "Monster.h"

STATUS CBTTask_Attack_Near::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);

	if (pMonster->GetAnimEnd(CMonster::ATTACK))
	{
		pMonster->Set_isAttack(true);

		pMonster->Get_GameInstance()->Play_Sound(TEXT("attack_impact_1"), SOUND_ATTACK, pMonster, 1.f, pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION));

		pMonster->Get_Target()->Add_Hp(-20.f);

		_float3 vForce{ pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK) };
		D3DXVec3Normalize(&vForce, &vForce);
		vForce *= 15.f;
		vForce.y += 15.f;

		pMonster->Get_Target()->Knock_back(vForce);

		return STATUS::SUCCESS;
	}

	pMonster->Set_Animation(CMonster::ATTACK);
	return STATUS::RUNNING;
}
