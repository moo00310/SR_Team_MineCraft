#pragma once
#include "Client_Defines.h"
#include "Sun.h"
#include "MCTerrain.h"
#include "Steve.h"
#include "FireCrackerLoad.h"

BEGIN(Client)
class CMissionControl :public CGameObject
{
public:
	enum Stage {DAY1, WAVE1, DAY2, WAVE2, DAY3, WAVE3, STAGE_END};
public:
	struct missionDetail {
		wstring name =L"";
		wstring word =L"";
		int count=0;
		int finishCount = 0;
		bool finish=false;
	};

	struct showMission {
		wstring word = L"";
		int curCount = 0;
		int endCount = 0;
	};
private:
	CMissionControl(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMissionControl(const CMissionControl& Prototype);
	virtual ~CMissionControl() = default;
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	void Update_Mission(wstring name);
	vector<showMission> Get_MissionList();
	bool Get_IsWave() { return m_bIsWave; }
	bool Get_IsWaveStart() { return m_bIsWaveStart; }
	int Get_CurrentStage() { return m_currentStage; }
private:
	void InitMissions1();
	void InitMissions2();
	void InitMissions3();
	float GetRandomFloat(float lowBound, float highBound);


	CSun* m_sun = nullptr;
	CMCTerrain* m_pTerrain = nullptr;
	CSteve* m_pPlayer = nullptr;

	bool m_bDayFinish = false;
	bool m_bNightFinish = false;
	bool m_bIsWaveStart = false;
	bool m_bIsWave = false;

	// ÆøÁ×.
	bool m_bIsFireCracker = false;
	float m_fFireTime = 0.f;

	// 2ÃÊ °£°ÝÀ¸·Î ÆøÁ× ¹ß»ç.
	const float m_fFireCoolTime = 2.f;

	// ¸¶Áö¸· wave À§ÇÑ 
	bool m_bIsLastWave = false;
	_float m_fSpwanCoolTime = {};
	vector<_float3> m_SpawnPos;
	void Spwan_Monster(int count);
	
	Stage m_currentStage= DAY1;
	vector<vector<missionDetail>> m_Round1;
	vector<vector<missionDetail>> m_Round1Wave;
	vector<vector<missionDetail>> m_Round2;
	vector<vector<missionDetail>> m_Round2Wave;
	vector<vector<missionDetail>> m_Round3;
	vector<vector<missionDetail>> m_Round3Wave;
public:
	static CMissionControl* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
END;