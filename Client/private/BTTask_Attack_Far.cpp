#include "BTTask_Attack_Far.h"
#include "Monster.h"

STATUS CBTTask_Attack_Far::Excute(CGameObject* _Obj, _float _fTimeDelta)
{
	CMonster* pMonster = static_cast<CMonster*>(_Obj);

	if (pMonster->GetAnimEnd(CMonster::ATTACK2))
	{
		pMonster->Set_isAttack(true);

		pMonster->Get_GameInstance()->Play_Sound(TEXT("sonic_boom1"), SOUND_ATTACK, pMonster, 1.f, pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION));

		CGameObject* pHitObject{ nullptr };

		pHitObject = pMonster->Get_GameInstance()->Ray_Cast(
			pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f, 1.f, 0.f },
			pMonster->Get_Target()->Get_Transform()->Get_State(CTransform::STATE_POSITION) - pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION),
			15.f,
			COLLISION_PLAYER
		);

		if (pHitObject)
		{
			static_cast<CPawn*>(pHitObject)->Add_Hp(-20.f);

			_float3 vForce{ pMonster->Get_Target()->Get_Transform()->Get_State(CTransform::STATE_POSITION) - pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION)};
			D3DXVec3Normalize(&vForce, &vForce);
			vForce *= 15.f;
			vForce.y += 15.f;


			static_cast<CPawn*>(pHitObject)->Knock_back(vForce);
		}

		return STATUS::SUCCESS;
	}

	pMonster->Set_Animation(CMonster::ATTACK2);
	return STATUS::RUNNING;
}
