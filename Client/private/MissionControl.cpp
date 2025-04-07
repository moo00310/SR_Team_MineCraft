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
        { L"����", 5 }
    });

    m_Round1Finish.emplace_back(std::vector<missionCheck>{
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
    });

    // ���̺� �̼� 1 : ���� 5���� óġ
    m_Round1WaveFinish.emplace_back(std::vector<missionCheck>{
        { L"����", 5 }
    });


    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"����", L"������ 5�� �����ϼ���"}
    });

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"���۴�", L"���۴븦 ���弼��"}
    });

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"��Į", L"�� Į�� ���弼��"},
        { L"������", L"�� ������ ���弼��"},
        { L"�����", L"�� ��̸� ���弼��"}
    });

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"���", L"����� �԰� ����� ä���"}
    });

    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"��", L"���� ��ġ�غ�����"}
    });

    // ���̺� �̼� 1 : ���� 5���� óġ
    m_Round1Wave.emplace_back(std::vector<missionDetail>{
        { L"����", L"���� 5������ ó���ϼ���"}
    });
}

void CMissionControl::InitMissions2()
{
    m_Round2Finish.emplace_back(std::vector<missionCheck>{
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
    });

    m_Round2WaveFinish.emplace_back(std::vector<missionCheck>{
        { L"����", 7 },
        { L"ũ����", 3 }
    });


    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"��ź", L"��ź�� ä���ϼ���"},
        { L"ö",   L"ö�� ä���ϼ���" }
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"ȭ��", L"ȭ�θ� ���弼��" },
        { L"ö����", L"ö�� ���켼��"}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"ö��", L"ö���� ���弼��"}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"ȶ��", L"��ź���� ȶ���� ���弼��"}
    });

    // ���̺� �̼� 1: ���� 7����, ũ���� 3���� óġ
    m_Round2Wave.emplace_back(std::vector<missionDetail>{
        { L"����", L"���� 7������ óġ�ϼ���"},
        { L"ũ����",  L"ũ���� 3������ óġ�ϼ���"}
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
        { L"����", L"���ο� ���⸦ ���弼��" }
    });

    // ���̺� �̼� 1: ���� ���� óġ & ����
    m_Round3Wave.emplace_back(std::vector<missionDetail>{
        { L"����", L"���� ���͸� ó���ϼ���" },
        { L"��ħ",  L"��ħ���� �����ϼ���" }
    });
}
#pragma endregion

void CMissionControl::Priority_Update(_float fTimeDelta)
{
    // �������� �ð� ������ ����
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