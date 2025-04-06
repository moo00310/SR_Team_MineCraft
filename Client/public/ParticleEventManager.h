#pragma once
#include "Base.h"
#include "PoolManager.h"
#include "ParticleSystem.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Transform.h"

// 파티클 적용 이벤트 매니저.
class CParticleEventManager final : public CBase
{
	DECLARE_SINGLETON(CParticleEventManager);
private:
	CParticleEventManager() = default;
	virtual ~CParticleEventManager() = default;

public:
	// 파티클 매니저 초기화.
	void Initialization(CGameInstance* _device);

	// 해당 태그 파티클 효과 발동.
	CParticleSystem* OnParticle(const _wstring& strPrototypeTag, CTransform* _transform, _float _yUp = 0.f);

	// 해당 태그 파티클 효과 발동.
	CParticleSystem* OnParticle(const _wstring& strPrototypeTag, _float3 _position);

	virtual void Free() override;

private:
	CGameInstance* m_pGameInstance = { nullptr };
};

