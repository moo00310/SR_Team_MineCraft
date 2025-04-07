#pragma once
#include "Client_Defines.h"

BEGIN(Client)
class CMissionControl :public CGameObject
{
public:
	struct missionDetail {
		wstring name =L"";
		wstring word =L"";
		int count=0;
		bool finish=false;
	};

	struct missionCheck {
		wstring name = L"";
		int count = 0;
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
private:
	void InitMissions1();
	void InitMissions2();
	void InitMissions3();

private:
	int m_currentStage=0;
	vector<vector<missionDetail>> m_Round1;
	vector<vector<missionCheck>> m_Round1Finish;
	vector<vector<missionDetail>> m_Round1Wave;
	vector<vector<missionCheck>> m_Round1WaveFinish;
	vector<vector<missionDetail>> m_Round2;
	vector<vector<missionCheck>> m_Round2Finish;
	vector<vector<missionDetail>> m_Round2Wave;
	vector<vector<missionCheck>> m_Round2WaveFinish;
	vector<vector<missionDetail>> m_Round3;
	vector<vector<missionCheck>> m_Round3Finish;
	vector<vector<missionDetail>> m_Round3Wave;
	vector<vector<missionCheck>> m_Round3WaveFinish;
public:
	static CMissionControl* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
END;