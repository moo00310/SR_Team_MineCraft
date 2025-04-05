#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "MCTerrain.h"
#include "Sun.h"

// 게임 시간과 게임 상태
// 몬스터 스폰을 관리하는 매니저

// 원래 게임이면 싱글톤으로 만들어야 하는데

class CGameMgr final : public CGameObject
{
private:
	CGameMgr(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameMgr(const CGameMgr& Prototype);
	virtual ~CGameMgr() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

private:
	void Spwan_Monster(int count);

private:
	CMCTerrain* m_pTerrain = { nullptr };
	CSun* m_pSun = { nullptr };

private:
	vector<_float3> m_SpawnPos;
	_float m_fSunAngle = {};
	int m_iMonsterCount = 0;
	_float m_fSpwanCoolTime = {};

public:
	static CGameMgr* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
