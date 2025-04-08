#include "MissionControl.h"
#include "MCTerrain.h"
#include "Monster.h"

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
    m_Round1Finish.emplace_back(std::vector<missionCheck>{
        { L"����", 5 }
    });

  /*  m_Round1Finish.emplace_back(std::vector<missionCheck>{
        { L"���۴�", 1 }
    });

    m_Round1Finish.emplace_back(std::vector<missionCheck>{
        { L"��Į", 1 },
        { L"������", 1 },
        { L"�����", 1 }
    });

    m_Round1Finish.emplace_back(std::vector<missionCheck>{
        { L"���", 1 }
    });

    m_Round1Finish.emplace_back(std::vector<missionCheck>{
        { L"��", 5 }
    });*/

    // ���̺� �̼� 1 : ���� 5���� óġ
    m_Round1WaveFinish.emplace_back(std::vector<missionCheck>{
        { L"����", 2 }
    });


    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"����", L"���� 5�� �����ϱ�"}
    });

    //m_Round1.emplace_back(std::vector<missionDetail>{
    //    { L"���۴�", L"���۴� �����"}
    //});

    //m_Round1.emplace_back(std::vector<missionDetail>{
    //    { L"��Į", L"�� Į �����"},
    //    { L"������", L"�� ���� �����"},
    //    { L"�����", L"�� ��� �����"}
    //});

    //m_Round1.emplace_back(std::vector<missionDetail>{
    //    { L"���", L"��� �԰� ����� ä���"}
    //});

    //m_Round1.emplace_back(std::vector<missionDetail>{
    //    { L"��", L"�� 5�� ��ġ"}
    //});

    // ���̺� �̼� 1 : ���� 5���� óġ
    m_Round1Wave.emplace_back(std::vector<missionDetail>{
        { L"����", L"���� 5���� ó��"}
    });
}
void CMissionControl::InitMissions2()
{
    m_Round2Finish.emplace_back(std::vector<missionCheck>{
        { L"����", 5 }
    });
           
    /*m_Round2Finish.emplace_back(std::vector<missionCheck>{
        { L"��ź", 5 },
        { L"ö", 5 }
    });

    m_Round2Finish.emplace_back(std::vector<missionCheck>{
        { L"ȭ��", 1 },
        { L"ö����", 2 }
    });

    m_Round2Finish.emplace_back(std::vector<missionCheck>{
        { L"ö��", 1 }
    });

    m_Round2Finish.emplace_back(std::vector<missionCheck>{
        { L"ȶ��", 1 }
    });*/

    m_Round2WaveFinish.emplace_back(std::vector<missionCheck>{
        { L"����", 7 },
        { L"ũ����", 3 }
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"����", L"���� 5�� �����ϱ�"},
    });
    /*m_Round2.emplace_back(std::vector<missionDetail>{
        { L"��ź", L"��ź ä����"},
        { L"ö",   L"ö ä���ϱ�" }
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"ȭ��", L"ȭ�� �����" },
        { L"ö����", L"ö �����"}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"ö��", L"ö�� �����"}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"ȶ��", L"��ź���� ȶ�� �����"}
    });*/

    // ���̺� �̼� 1: ���� 7����, ũ���� 3���� óġ
    m_Round2Wave.emplace_back(std::vector<missionDetail>{
        { L"����", L"���� 7���� óġ"},
        { L"ũ����",  L"ũ���� 3���� óġ"}
    });
}
void CMissionControl::InitMissions3()
{
    m_Round3Finish.emplace_back(std::vector<missionCheck>{
        { L"����", 1 }
    });

    m_Round3WaveFinish.emplace_back(std::vector<missionCheck>{
        { L"����", 1 },
        { L"��ħ", 1 }
    });


    m_Round3.emplace_back(std::vector<missionDetail>{
        { L"����", L"���ο� ���� �����" }
    });

    // ���̺� �̼� 1: ���� ���� óġ & ����
    m_Round3Wave.emplace_back(std::vector<missionDetail>{
        { L"����", L"���� ���� ó��" },
        { L"��ħ",  L"��ħ���� �����ϱ�" }
    });
}
#pragma endregion

void CMissionControl::Priority_Update(_float fTimeDelta)
{
    // �������� �ð� ������ ����
    if (m_bDayFinish) {
        if (g_fBright <= 0.25) {
            m_sun->Set_bAddTime();
            m_bDayFinish = false;
            for (int i = 0; i < 2; i++)
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
    case 0:
        for (int i = 0; i < m_Round1Finish.size(); ++i) {
            for (int j = 0; j < m_Round1Finish[i].size(); ++j) {
                if (m_Round1[i][j].name == m_Round1Finish[i][j].name) {
                    if (m_Round1[i][j].count == m_Round1Finish[i][j].count) {
                        if (!m_Round1[i][j].finish) {
                            m_pGameInstance->PlaySound(TEXT("ding"), 0.5f, m_pPlayer->GetPos());
                        }
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
            m_bDayFinish = true;
            m_pGameInstance->PlayBGM(L"MoogCity2");
            m_sun->Set_bAddTime();     
            m_bIsWaveStart = true;

        }
        break;
    case 1:
        for (int i = 0; i < m_Round1WaveFinish.size(); ++i) {
            for (int j = 0; j < m_Round1WaveFinish[i].size(); ++j) {
                if (m_Round1Wave[i][j].name == m_Round1WaveFinish[i][j].name) {
                    if (m_Round1Wave[i][j].count == m_Round1WaveFinish[i][j].count) {
                        m_Round1Wave[i][j].finish = true;
                        m_pGameInstance->PlaySound(TEXT("ding"), 0.5f, m_pPlayer->GetPos());
                    }
                    else {
                        m_checkFinishStage = false;
                    }
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage++;
            m_bNightFinish = true;
            m_pGameInstance->PlayBGM(L"sweden");
            m_sun->Set_bAddTime();
            m_bIsWave = false;
        }
        break;
    case 2:
        for (int i = 0; i < m_Round2Finish.size(); ++i) {
            for (int j = 0; j < m_Round2Finish[i].size(); ++j) {
                if (m_Round2[i][j].name == m_Round2Finish[i][j].name) {
                    if (m_Round2[i][j].count == m_Round2Finish[i][j].count) {
                        m_Round2[i][j].finish = true;
                        m_pGameInstance->PlaySound(TEXT("ding"), 0.5f, m_pPlayer->GetPos());
                    }
                    else {
                        m_checkFinishStage = false;
                    }
                }
            }
        }

        if (m_checkFinishStage) {
            m_currentStage++;
            m_bDayFinish = true;
            m_pGameInstance->PlayBGM(L"MoogCity2");
            m_sun->Set_bAddTime();
            m_bIsWaveStart = true;
        }
        break;
    case 3:
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
    case 0:
        for (int i = 0; i < m_Round1.size(); ++i) {
            for (int j = 0; j < m_Round1[i].size(); ++j) {
                showMission _mission = { m_Round1[i][j].word, m_Round1[i][j].count, m_Round1Finish[i][j].count };
                _vec.push_back(_mission);
            }
        }
        break;
    case 1:
        for (int i = 0; i < m_Round1Wave.size(); ++i) {
            for (int j = 0; j < m_Round1Wave[i].size(); ++j) {
                showMission _mission = { m_Round1Wave[i][j].word, m_Round1Wave[i][j].count, m_Round1WaveFinish[i][j].count };
                _vec.push_back(_mission);
            }
        }
        break;
    case 2:
        for (int i = 0; i < m_Round2.size(); ++i) {
            for (int j = 0; j < m_Round2[i].size(); ++j) {
                showMission _mission = { m_Round2[i][j].word, m_Round2[i][j].count, m_Round2Finish[i][j].count };
                _vec.push_back(_mission);
            }
        }
        break;
    case 3:
        for (int i = 0; i < m_Round3.size(); ++i) {
            for (int j = 0; j < m_Round3[i].size(); ++j) {
                showMission _mission = { m_Round3[i][j].word, m_Round3[i][j].count, m_Round3Finish[i][j].count };
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