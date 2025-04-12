#include "MissionControl.h"
#include "MCTerrain.h"
#include "Monster.h"
#include "Camera_Cutscene.h"

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
    if (CMCTerrain* _terrain = dynamic_cast<CMCTerrain*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Terrain")))) {
        m_pTerrain = _terrain;
    }
    if (CSteve* _terrain = dynamic_cast<CSteve*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve")))) {
        m_pPlayer = _terrain;
    }

	return S_OK;
}

#pragma region init

void CMissionControl::InitMissions1() {
    // 무엇을, 어떻게 하냐, 지금 한 값, 완료되는 값  
    //m_Round1.emplace_back(std::vector<missionDetail>{
    //    { L"나무", L"나무 5개 벌목하기",0, 5}
    //});
    //
    //m_Round1.emplace_back(std::vector<missionDetail>{
    //    { L"제작대", L"제작대 만들기",0,1}
    //});
    /*

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"돌칼", L"돌 칼 만들기",0,1},
        { L"돌도끼", L"돌 도끼 만들기",0,1},
        { L"돌곡괭이", L"돌 곡괭이 만들기",0,1}
    });

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"사과", L"사과 먹고 배고픔 채우기",0,1}
    });

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"블럭", L"블럭 5개 설치",0,5}
    });*/

    // 웨이브 미션 1 : 좀비 5마리 처치
    m_Round1Wave.emplace_back(std::vector<missionDetail>{
        { L"좀비", L"좀비 5마리 처지",0,5}
    });
}
void CMissionControl::InitMissions2()
{
    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"석탄", L"석탄 채광기",0,3},
        { L"철",   L"철 채광하기" ,0,3}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"화로", L"화로 만들기" ,0,1},
        { L"철굽기", L"철 구우기",0,2}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"철검", L"철검 만들기",0,1}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"횃불", L"석탄으로 횃불 만들기",0,1}
    });

    // 웨이브 미션 1: 좀비 7마리, 크리퍼 3마리 처치
    m_Round2Wave.emplace_back(std::vector<missionDetail>{
        { L"좀비", L"좀비 7마리 처치",0,7},
        { L"크리퍼",  L"크리퍼 3마리 처치",0,3}
    });
}
void CMissionControl::InitMissions3()
{
    m_Round3.emplace_back(std::vector<missionDetail>{
        { L"무기", L"새로운 무기 만들기" ,0,1}
    });

    // 웨이브 미션 1: 보스 몬스터 처치 & 생존
    m_Round3Wave.emplace_back(std::vector<missionDetail>{
        { L"보스", L"보스 몬스터 처지" ,0,1},
        { L"아침",  L"아침까지 생존하기" ,0,1}
    });
}
#pragma endregion

void CMissionControl::Priority_Update(_float fTimeDelta)
{
    // 끝났으면 시간 빠르게 가기
    if (m_bDayFinish) {
        if (g_fBright <= 0.25) {
            m_sun->Set_bAddTime();
            m_bDayFinish = false;
            switch (m_currentStage)
            {
            case Client::CMissionControl::WAVE1:
            {
                vector<_float3> m_SpawnPos = m_pTerrain->Get_SpwanAble();

                for (int i = 0; i < 5; i++)
                {
                    int Random_pos = rand() % m_SpawnPos.size();
                    CGameObject* ptemp = nullptr;

                    ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Zombi"),
                        LEVEL_YU, TEXT("Layer_Monster"));

                    if (ptemp == nullptr) return;
                    static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));
                    m_bIsWaveStart = false;
                    m_bIsWave = true;
                }

                int Random_pos = rand() % m_SpawnPos.size();
                CGameObject* ptemp = nullptr;

                ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Warden"),
                    LEVEL_YU, TEXT("Layer_Monster"));

                if (ptemp == nullptr) return;
                static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));

                static_cast<Camera_Cutscene*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Camera_Cutscene")))->Start_Cutscene(m_SpawnPos[Random_pos]);

                m_bIsWaveStart = false;
                m_bIsWave = true;

                break;
            }

            case Client::CMissionControl::WAVE2:
                for (int i = 0; i < 5; i++)
                {
                    vector<_float3> m_SpawnPos = m_pTerrain->Get_SpwanAble();
                    int Random_pos = rand() % m_SpawnPos.size();
                    CGameObject* ptemp = nullptr;

                    ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Zombi"),
                        LEVEL_YU, TEXT("Layer_Monster"));

                    if (ptemp == nullptr) return;
                    static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));
                    m_bIsWaveStart = false;
                    m_bIsWave = true;
                }
                break;
            case Client::CMissionControl::WAVE3:
                for (int i = 0; i < 5; i++)
                {
                    vector<_float3> m_SpawnPos = m_pTerrain->Get_SpwanAble();
                    int Random_pos = rand() % m_SpawnPos.size();
                    CGameObject* ptemp = nullptr;

                    ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Zombi"),
                        LEVEL_YU, TEXT("Layer_Monster"));

                    if (ptemp == nullptr) return;
                    static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));
                    m_bIsWaveStart = false;
                    m_bIsWave = true;
                }
                break;
            }
        }
    }

    if (m_bNightFinish) {
        if (g_fBright >= 0.75) {
            m_sun->Set_bAddTime();
            m_bNightFinish = false;
        }
    }

}

void CMissionControl::Update(_float fTimeDelta)
{
    bool m_checkFinishStage = true;
    switch (m_currentStage)
    {
    case DAY1:
        for (int i = 0; i < m_Round1.size(); ++i) {
            for (int j = 0; j < m_Round1[i].size(); ++j) {
                if (m_Round1[i][j].count == m_Round1[i][j].finishCount) {
                    if (m_Round1[i][j].finish)
                        continue;
                    m_Round1[i][j].finish = true;
                    m_pGameInstance->Play_Sound(TEXT("ding"), SOUND_DING, this, 0.5f, m_pPlayer->GetPos());
                }
                else {
                    m_checkFinishStage = false;
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage = WAVE1;
            m_bDayFinish = true;
            m_pGameInstance->Play_Sound(TEXT("raidhorn_02"), SOUND_EVENT, this, 1.0f, m_pPlayer->GetPos());
            m_pGameInstance->PlayBGM(L"MoogCity2");
            m_sun->Set_bAddTime();     
            m_bIsWaveStart = true;

        }
        break;
    case WAVE1:
        for (int i = 0; i < m_Round1Wave.size(); ++i) {
            for (int j = 0; j < m_Round1Wave[i].size(); ++j) {
                if (m_Round1Wave[i][j].count == m_Round1Wave[i][j].finishCount) {
                    if (m_Round1Wave[i][j].finish)
                        continue;
                    m_Round1Wave[i][j].finish = true;
                    m_pGameInstance->Play_Sound(TEXT("ding"), SOUND_DING, this, 0.5f, m_pPlayer->GetPos());
                }else {
                   m_checkFinishStage = false;
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage = DAY2;
            m_bNightFinish = true;
            m_pGameInstance->PlayBGM(L"sweden");
            m_sun->Set_bAddTime();
            m_bIsWave = false;
        }
        break;
    case DAY2:
        for (int i = 0; i < m_Round2.size(); ++i) {
            for (int j = 0; j < m_Round2[i].size(); ++j) {
                if (m_Round2[i][j].count == m_Round2[i][j].finishCount) {
                    if (m_Round2[i][j].finish)
                        continue;
                    m_Round2[i][j].finish = true;
                    m_pGameInstance->Play_Sound(TEXT("ding"), SOUND_DING, this, 0.5f, m_pPlayer->GetPos());
                }
                else {
                    m_checkFinishStage = false;
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage = WAVE2;
            m_bDayFinish = true;
            m_pGameInstance->Play_Sound(TEXT("raidhorn_02"), SOUND_EVENT, this, 1.0f, m_pPlayer->GetPos());
            m_pGameInstance->PlayBGM(L"MoogCity2");
            m_sun->Set_bAddTime();
            m_bIsWaveStart = true;
        }
        break;
    case WAVE2:
        for (int i = 0; i < m_Round2Wave.size(); ++i) {
            for (int j = 0; j < m_Round2Wave[i].size(); ++j) {
                if (m_Round2Wave[i][j].count == m_Round2Wave[i][j].finishCount) {
                    if (m_Round2Wave[i][j].finish)
                        continue;
                    m_Round2Wave[i][j].finish = true;
                    m_pGameInstance->Play_Sound(TEXT("ding"), SOUND_DING, this, 0.5f, m_pPlayer->GetPos());
                }
                else {
                    m_checkFinishStage = false;
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage = DAY3;
            m_bNightFinish = true;
            m_pGameInstance->PlayBGM(L"sweden");
            m_sun->Set_bAddTime();
            m_bIsWave = false;
        }
        break;
    case DAY3:
        for (int i = 0; i < m_Round3.size(); ++i) {
            for (int j = 0; j < m_Round3[i].size(); ++j) {
                if (m_Round3[i][j].count == m_Round3[i][j].finishCount) {
                    if (m_Round3[i][j].finish)
                        continue;
                    m_Round2Wave[i][j].finish = true;
                    m_pGameInstance->Play_Sound(TEXT("ding"), SOUND_DING, this, 0.5f, m_pPlayer->GetPos());
                }
                else {
                    m_checkFinishStage = false;
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage = WAVE3;
            m_bDayFinish = true;
            m_pGameInstance->Play_Sound(TEXT("raidhorn_02"), SOUND_EVENT, this, 1.0f, m_pPlayer->GetPos());
            m_pGameInstance->PlayBGM(L"MoogCity2");
            m_sun->Set_bAddTime();
            m_bIsWaveStart = true;
        }
        break;
    case WAVE3:
        for (int i = 0; i < m_Round3Wave.size(); ++i) {
            for (int j = 0; j < m_Round3Wave[i].size(); ++j) {
                if (m_Round3Wave[i][j].count == m_Round3Wave[i][j].finishCount) {
                    if (m_Round3Wave[i][j].finish)
                        continue;
                    m_Round3Wave[i][j].finish = true;
                    m_pGameInstance->Play_Sound(TEXT("ding"), SOUND_DING, this, 0.5f, m_pPlayer->GetPos());
                }
                else {
                    m_checkFinishStage = false;
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage = STAGE_END;
            m_bNightFinish = true;
            m_pGameInstance->PlayBGM(L"sweden");
            m_sun->Set_bAddTime();
            m_bIsWave = false;
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
        for (int i = 0; i < m_Round1Wave.size(); ++i) {
            for (int j = 0; j < m_Round1Wave[i].size(); ++j) {
                if (m_Round1Wave[i][j].name == name) {
                    m_Round1Wave[i][j].count++;
                }
            }
        }
        break;
    case 2:
        for (int i = 0; i < m_Round2.size(); ++i) {
            for (int j = 0; j < m_Round2[i].size(); ++j) {
                if (m_Round2[i][j].name == name) {
                    m_Round2[i][j].count++;
                }
            }
        }
        break;
    default:
        break;
    }
}

vector<CMissionControl::showMission> CMissionControl::Get_MissionList()
{
    vector<showMission> _vec;
    switch (m_currentStage)
    {
    case DAY1:
        for (int i = 0; i < m_Round1.size(); ++i) {
            for (int j = 0; j < m_Round1[i].size(); ++j) {
                showMission _mission = { m_Round1[i][j].word, m_Round1[i][j].count, m_Round1[i][j].finishCount };
                _vec.push_back(_mission);
            }
        }
        break;
    case WAVE1:
        for (int i = 0; i < m_Round1Wave.size(); ++i) {
            for (int j = 0; j < m_Round1Wave[i].size(); ++j) {
                showMission _mission = { m_Round1Wave[i][j].word, m_Round1Wave[i][j].count, m_Round1Wave[i][j].finishCount };
                _vec.push_back(_mission);
            }
        }
        break;
    case DAY2:
        for (int i = 0; i < m_Round2.size(); ++i) {
            for (int j = 0; j < m_Round2[i].size(); ++j) {
                showMission _mission = { m_Round2[i][j].word, m_Round2[i][j].count, m_Round2[i][j].finishCount };
                _vec.push_back(_mission);
            }
        }
        break;
    case WAVE2:
        for (int i = 0; i < m_Round2Wave.size(); ++i) {
            for (int j = 0; j < m_Round2Wave[i].size(); ++j) {
                showMission _mission = { m_Round2Wave[i][j].word, m_Round2Wave[i][j].count, m_Round2Wave[i][j].finishCount };
                _vec.push_back(_mission);
            }
        }
        break;
    case DAY3:
        for (int i = 0; i < m_Round3.size(); ++i) {
            for (int j = 0; j < m_Round3[i].size(); ++j) {
                showMission _mission = { m_Round3[i][j].word, m_Round3[i][j].count, m_Round3[i][j].finishCount };
                _vec.push_back(_mission);
            }
        }
        break;
    case WAVE3:
        for (int i = 0; i < m_Round3Wave.size(); ++i) {
            for (int j = 0; j < m_Round3Wave[i].size(); ++j) {
                showMission _mission = { m_Round3Wave[i][j].word, m_Round3Wave[i][j].count, m_Round3Wave[i][j].finishCount };
                _vec.push_back(_mission);
            }
        }
        break;
    default:
        break;
    }

    return _vec;
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