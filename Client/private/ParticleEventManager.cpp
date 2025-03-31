#include "ParticleEventManager.h"

IMPLEMENT_SINGLETON(CParticleEventManager)

void CParticleEventManager::Initialization(CGameInstance* _device)
{
	m_pGameInstance = _device;

	Safe_AddRef(m_pGameInstance);
}

void CParticleEventManager::OnParticle(const _wstring& strPrototypeTag, CTransform* _transform)
{
	CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// ������ ��.
		strPrototypeTag,	// ������ ������Ÿ��.
		LEVEL_STATIC,		// ���� ��.
		LAYER_PARTICLE);	// �ֵ������Ʈ�� �߰��� ���̾�

	// ��ƼŬ Ǯ ��ü null üũ.
	if (particle == nullptr)
	{
		MSG_BOX("��ƼŬ ��ü�� �����ϱ� ���� Ǯ ��ü�� �����ϴ�");
		return;
	}

	// ��ƼŬ ����.
	particle->GetTransform()->Set_State(CTransform::STATE_LOOK, _transform->Get_State(CTransform::STATE_LOOK));
	particle->GetTransform()->Set_State(CTransform::STATE_UP, _transform->Get_State(CTransform::STATE_UP));
	particle->GetTransform()->Set_State(CTransform::STATE_RIGHT, _transform->Get_State(CTransform::STATE_RIGHT));
	
	_float3 _pos = _transform->Get_State(CTransform::STATE_POSITION);
	_pos.y += 0.2f;
	particle->Replay(_pos);
}

void CParticleEventManager::OnParticle(const _wstring& strPrototypeTag, _float3 _position)
{
	CParticleSystem* particle = (CParticleSystem*)m_pGameInstance->PushPool(LEVEL_STATIC,	// ������ ��.
		strPrototypeTag,	// ������ ������Ÿ��.
		LEVEL_STATIC,		// ���� ��.
		LAYER_PARTICLE);	// �ֵ������Ʈ�� �߰��� ���̾�

	// ��ƼŬ Ǯ ��ü null üũ.
	if (particle == nullptr)
	{
		MSG_BOX("��ƼŬ ��ü�� �����ϱ� ���� Ǯ ��ü�� �����ϴ�");
		return;
	}

	// ��ƼŬ ����.	
	particle->Replay(_position);
}

void CParticleEventManager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
