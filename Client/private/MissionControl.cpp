#include "MissionControl.h"

CMissionControl::CMissionControl(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CMissionControl::CMissionControl(const CMissionControl& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CMissionControl::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMissionControl::Initialize(void* pArg)
{
    InitMissions1();
    InitMissions2();
    InitMissions3();

    if (CSun* _sun = dynamic_cast<CSun*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Sun")))) {
        m_sun = _sun;
    }
	return S_OK;
}

#pragma region init

void CMissionControl::InitMissions1() {
    m_Round1Finish.emplace_back(std::vector<missionCheck>{
        { L"나무", 5 }
    });

    m_Round1Finish.emplace_back(std::vector<missionCheck>{
        { L"제작대", 1 }
    });

    m_Round1Finish.emplace_back(std::vector<missionCheck>{
        { L"돌칼", 1 },
        { L"돌도끼", 1 },
        { L"돌곡괭이", 1 }
    });

    m_Round1Finish.emplace_back(std::vector<missionCheck>{
        { L"사과", 1 }
    });

    m_Round1Finish.emplace_back(std::vector<missionCheck>{
        { L"블럭", 5 }
    });

    // 웨이브 미션 1 : 좀비 5마리 처치
    m_Round1WaveFinish.emplace_back(std::vector<missionCheck>{
        { L"좀비", 5 }
    });


    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"나무", L"나무를 5개 벌목하세요"}
    });

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"제작대", L"제작대를 만드세요"}
    });

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"돌칼", L"돌 칼을 만드세요"},
        { L"돌도끼", L"돌 도끼를 만드세요"},
        { L"돌곡괭이", L"돌 곡괭이를 만드세요"}
    });

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"사과", L"사과를 먹고 배고픔 채우기"}
    });

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"블럭", L"블럭을 설치해보세요"}
    });

    // 웨이브 미션 1 : 좀비 5마리 처치
    m_Round1Wave.emplace_back(std::vector<missionDetail>{
        { L"좀비", L"좀비 5마리를 처지하세요"}
    });
}

void CMissionControl::InitMissions2()
{
    m_Round2Finish.emplace_back(std::vector<missionCheck>{
        { L"석탄", 5 },
        { L"철", 5 }
    });

    m_Round2Finish.emplace_back(std::vector<missionCheck>{
        { L"화로", 1 },
        { L"철굽기", 2 }
    });

    m_Round2Finish.emplace_back(std::vector<missionCheck>{
        { L"철검", 1 }
    });

    m_Round2Finish.emplace_back(std::vector<missionCheck>{
        { L"횃불", 1 }
    });

    m_Round2WaveFinish.emplace_back(std::vector<missionCheck>{
        { L"좀비", 7 },
        { L"크리퍼", 3 }
    });


    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"석탄", L"석탄을 채광하세요"},
        { L"철",   L"철을 채광하세요" }
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"화로", L"화로를 만드세요" },
        { L"철굽기", L"철을 구우세요"}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"철검", L"철검을 만드세요"}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"횃불", L"석탄으로 횃불을 만드세요"}
    });

    // 웨이브 미션 1: 좀비 7마리, 크리퍼 3마리 처치
    m_Round2Wave.emplace_back(std::vector<missionDetail>{
        { L"좀비", L"좀비 7마리를 처치하세요"},
        { L"크리퍼",  L"크리퍼 3마리를 처치하세요"}
    });
}
void CMissionControl::InitMissions3()
{
    m_Round3Finish.emplace_back(std::vector<missionCheck>{
        { L"무기", 1 }
    });

    m_Round3WaveFinish.emplace_back(std::vector<missionCheck>{
        { L"보스", 1 },
        { L"아침", 1 }
    });


    m_Round3.emplace_back(std::vector<missionDetail>{
        { L"무기", L"새로운 무기를 만드세요" }
    });

    // 웨이브 미션 1: 보스 몬스터 처치 & 생존
    m_Round3Wave.emplace_back(std::vector<missionDetail>{
        { L"보스", L"보스 몬스터를 처지하세요" },
        { L"아침",  L"아침까지 생존하세요" }
    });
}
#pragma endregion

void CMissionControl::Priority_Update(_float fTimeDelta)
{
    // 끝났으면 시간 빠르게 가기
    if (m_bFinish) {
        m_sun->Set_bAddTime();
        if (g_fBright <= 0.25) {
            m_sun->Set_bAddTime();
            m_bFinish = false;
        }
    }
}

void CMissionControl::Update(_float fTimeDelta)
{
    bool m_checkFinishStage = true;
    switch (m_currentStage)
    {
    case 0:
        for (int i = 0; i < m_Round1Finish.size(); ++i) {
            for (int j = 0; j < m_Round1Finish[i].size(); ++j) {
                if (m_Round1[i][j].name == m_Round1Finish[i][j].name) {
                    if (m_Round1[i][j].count == m_Round1Finish[i][j].count) {
                        m_Round1[i][j].finish = true;
                    }
                    else {
                        m_checkFinishStage = false;
                    }
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage++;
            m_bFinish = true;
        }
        break;
    case 1:
        for (int i = 0; i < m_Round2Finish.size(); ++i) {
            for (int j = 0; j < m_Round2Finish[i].size(); ++j) {
                if (m_Round2[i][j].name == m_Round2Finish[i][j].name) {
                    if (m_Round2[i][j].count == m_Round2Finish[i][j].count) {
                        m_Round2[i][j].finish = true;
                    }
                    else {
                        m_checkFinishStage = false;
                    }
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage++;
        }
        break;
    case 2:
        for (int i = 0; i < m_Round3Finish.size(); ++i) {
            for (int j = 0; j < m_Round3Finish[i].size(); ++j) {
                if (m_Round3[i][j].name == m_Round3Finish[i][j].name) {
                    if (m_Round3[i][j].count == m_Round3Finish[i][j].count) {
                        m_Round3[i][j].finish = true;
                    }
                    else {
                        m_checkFinishStage = false;
                    }
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage++;
        }
        break;
    default:
        break;
    }
}

void CMissionControl::Late_Update(_float fTimeDelta)
{

}

void CMissionControl::Update_Mission(wstring name)
{
    switch (m_currentStage)
    {
    case 0:
        for (int i = 0; i < m_Round1.size(); ++i) {
            for (int j = 0; j < m_Round1[i].size(); ++j) {
                if (m_Round1[i][j].name == name) {
                    m_Round1[i][j].count++;
                }
            }
        }
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        break;
    }
}

CMissionControl* CMissionControl::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMissionControl* pInstance = new CMissionControl(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMissionControl");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMissionControl::Clone(void* pArg)
{
	CMissionControl* pInstance = new CMissionControl(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CMissionControl");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMissionControl::Free()
{
	__super::Free();
}