#include "ParticleEventManager.h"

IMPLEMENT_SINGLETON(CParticleEventManager)

void CParticleEventManager::Initialization(CGameInstance* _device)
{
	m_pGameInstance = _device;

	Safe_AddRef(m_pGameInstance);
}

void CParticleEventManager::OnParticle(const _wstring& strPrototypeTag, CTransform* _transform)
{
	CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// 가져올 씬.
		strPrototypeTag,	// 가져올 프로토타입.
		LEVEL_STATIC,		// 적용 씬.
		LAYER_PARTICLE);	// 애드오브젝트에 추가할 레이어

	// 파티클 풀 객체 null 체크.
	if (particle == nullptr)
	{
		MSG_BOX("파티클 객체를 생성하기 위한 풀 객체가 없습니다");
		return;
	}

	// 파티클 적용.
	particle->GetTransform()->Set_State(CTransform::STATE_LOOK, _transform->Get_State(CTransform::STATE_LOOK));
	particle->GetTransform()->Set_State(CTransform::STATE_UP, _transform->Get_State(CTransform::STATE_UP));
	particle->GetTransform()->Set_State(CTransform::STATE_RIGHT, _transform->Get_State(CTransform::STATE_RIGHT));
	
	_float3 _pos = _transform->Get_State(CTransform::STATE_POSITION);
	_pos.y += 0.2f;
	particle->Replay(_pos);
}

void CParticleEventManager::OnParticle(const _wstring& strPrototypeTag, _float3 _position)
{
	CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// 가져올 씬.
		strPrototypeTag,	// 가져올 프로토타입.
		LEVEL_STATIC,		// 적용 씬.
		LAYER_PARTICLE);	// 애드오브젝트에 추가할 레이어

	// 파티클 풀 객체 null 체크.
	if (particle == nullptr)
	{
		MSG_BOX("파티클 객체를 생성하기 위한 풀 객체가 없습니다");
		return;
	}

	// 파티클 적용.	
	particle->Replay(_position);
}

void CParticleEventManager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
