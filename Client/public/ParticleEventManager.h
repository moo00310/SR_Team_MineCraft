#pragma once
#include "Base.h"
#include "PoolManager.h"
#include "ParticleSystem.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Transform.h"

// ��ƼŬ ���� �̺�Ʈ �Ŵ���.
class CParticleEventManager final : public CBase
{
	DECLARE_SINGLETON(CParticleEventManager);
private:
	CParticleEventManager() = default;
	virtual ~CParticleEventManager() = default;

public:
	// ��ƼŬ �Ŵ��� �ʱ�ȭ.
	void Initialization(CGameInstance* _device);

	// �ش� �±� ��ƼŬ ȿ�� �ߵ�.
	CParticleSystem* OnParticle(const _wstring& strPrototypeTag, CTransform* _transform, _float _yUp = 0.f);

	// �ش� �±� ��ƼŬ ȿ�� �ߵ�.
	CParticleSystem* OnParticle(const _wstring& strPrototypeTag, _float3 _position);

	virtual void Free() override;

private:
	CGameInstance* m_pGameInstance = { nullptr };
};

