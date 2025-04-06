#include "ParticleEventManager.h"

IMPLEMENT_SINGLETON(CParticleEventManager)

void CParticleEventManager::Initialization(CGameInstance* _device)
{
	m_pGameInstance = _device;

	Safe_AddRef(m_pGameInstance);
}

CParticleSystem* CParticleEventManager::OnParticle(const _wstring& strPrototypeTag, CTransform* _transform, _float _yUp)
{
	CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// ������ ��.
		strPrototypeTag,	// ������ ������Ÿ��.
		LEVEL_STATIC,		// ���� ��.
		LAYER_PARTICLE);	// �ֵ������Ʈ�� �߰��� ���̾�

	// ��ƼŬ Ǯ ��ü null üũ.
	if (particle == nullptr)
	{
		MSG_BOX("��ƼŬ ��ü�� �����ϱ� ���� Ǯ ��ü�� �����ϴ�");
		return nullptr;
	}

	// ��ƼŬ ����.
	particle->GetTransform()->Set_State(CTransform::STATE_LOOK, _transform->Get_State(CTransform::STATE_LOOK));
	particle->GetTransform()->Set_State(CTransform::STATE_UP, _transform->Get_State(CTransform::STATE_UP));
	particle->GetTransform()->Set_State(CTransform::STATE_RIGHT, _transform->Get_State(CTransform::STATE_RIGHT));
	
	_float3 _pos = _transform->Get_State(CTransform::STATE_POSITION);
	_pos.y += _yUp;

	particle->Replay(_pos);

	return particle;
}

CParticleSystem* CParticleEventManager::OnParticle(const _wstring& strPrototypeTag, _float3 _position)
{
	CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// ������ ��.
		strPrototypeTag,	// ������ ������Ÿ��.
		LEVEL_STATIC,		// ���� ��.
		LAYER_PARTICLE);	// �ֵ������Ʈ�� �߰��� ���̾�

	// ��ƼŬ Ǯ ��ü null üũ.
	if (particle == nullptr)
	{
		MSG_BOX("��ƼŬ ��ü�� �����ϱ� ���� Ǯ ��ü�� �����ϴ�");
		return nullptr;
	}

	// ��ƼŬ ����.	
	particle->Replay(_position);
	
	return particle;
}

void CParticleEventManager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
