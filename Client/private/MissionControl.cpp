#include "MissionControl.h"
#include "MCTerrain.h"
#include "Monster.h"
#include "Camera_Player.h"

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
    // ������, ��� �ϳ�, ���� �� ��, �Ϸ�Ǵ� ��  
    m_Round1.emplace_back(std::vector<missionDetail>{
        { L"wood", L"���� 5�� �����ϱ�",0, 1}
    });
    
    //m_Round1.emplace_back(std::vector<missionDetail>{
    //    { L"craftingtable", L"���۴� �����",0,1}
    //});
    //
    //m_Round1.emplace_back(std::vector<missionDetail>{
    //    { L"stonesword", L"�� Į �����",0,1},
    //    { L"stoneaxe", L"�� ���� �����",0,1},
    //    { L"stonepickaxe", L"�� ��� �����",0,1}
    //});

    //m_Round1.emplace_back(std::vector<missionDetail>{
    //    { L"apple", L"��� �԰� ����� ä���",0,1}
    //});

    //m_Round1.emplace_back(std::vector<missionDetail>{
    //    { L"block", L"�� 5�� ��ġ",0,5}
    //});

    // ���̺� �̼� 1 : ���� 5���� óġ
    m_Round1Wave.emplace_back(std::vector<missionDetail>{
        { L"zombi", L"���� 5���� ó��",0,1}
    });
}
void CMissionControl::InitMissions2()
{
 /*   m_Round2.emplace_back(std::vector<missionDetail>{
        { L"coal", L"��ź ä����",0,3},
        { L"iron",   L"ö ä���ϱ�" ,0,5}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"furnace", L"ȭ�� �����" ,0,1},
        { L"burniron", L"ö �����",0,2}
    });

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"ironsword", L"ö�� �����",0,1}
    });*/

    m_Round2.emplace_back(std::vector<missionDetail>{
        { L"torch", L"��ź���� ȶ�� �����",0,1}
    });

    // ���̺� �̼� 1: ���� 7����, ũ���� 3���� óġ
    m_Round2Wave.emplace_back(std::vector<missionDetail>{
        { L"zombi", L"���� 7���� óġ",0,1},
        //{ L"creeper",  L"ũ���� 3���� óġ",0,1}
    });
}
void CMissionControl::InitMissions3()
{
    m_Round3.emplace_back(std::vector<missionDetail>{
        { L"block", L"���ο� ���� �����" ,0,1}
    });

    // ���̺� �̼� 1: ���� ���� óġ & ����
    m_Round3Wave.emplace_back(std::vector<missionDetail>{
        { L"warden", L"���� ���� ó��" ,0,1},
        { L"morning",  L"��ħ���� �����ϱ�" ,0,1}
    });
}
float CMissionControl::GetRandomFloat(float lowBound, float highBound)
{
    // �߸��� �Է� 
    if (lowBound >= highBound)
    {
        return lowBound;
    }

    // [0, 1] ������ ������ float ȹ��.
    float f = (rand() % 10000) * 0.0001f;

    // ���������� lowBound ~ highBound ���� �� ����.
    return (f * (highBound - lowBound)) + lowBound;
}
#pragma endregion

void CMissionControl::Priority_Update(_float fTimeDelta)
{
    // �������� �ð� ������ ����
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

                }

                m_bIsWaveStart = false;
                m_bIsWave = true;

                break;
            }
            case Client::CMissionControl::WAVE2:
            {
                m_bIsWaveStart = false;
                m_bIsWave = true;
                vector<_float3> m_SpawnPos = m_pTerrain->Get_SpwanAble();
                for (int i = 0; i < 7; i++)
                {
                    int Random_pos = rand() % m_SpawnPos.size();
                    CGameObject* ptemp = nullptr;

                    ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Zombi"),
                        LEVEL_YU, TEXT("Layer_Monster"));

                    if (ptemp == nullptr) return;
                    static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));
                }
                for (int i = 0; i < 4; i++)
                {
                    int Random_pos = rand() % m_SpawnPos.size();
                    CGameObject* ptemp = nullptr;

                    ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Creeper"),
                        LEVEL_YU, TEXT("Layer_Monster"));

                    if (ptemp == nullptr) return;
                    static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));
                }
                break;
            }
            case Client::CMissionControl::WAVE3:
            {
                m_bIsWaveStart = false;
                m_bIsWave = true;
                m_sun->Set_bLastWave();
                vector<_float3> m_SpawnPos = m_pTerrain->Get_SpwanAble();
                for (int i = 0; i < 15; i++)
                {
                    int Random_pos = rand() % m_SpawnPos.size();
                    CGameObject* ptemp = nullptr;

                    ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Zombi"),
                        LEVEL_YU, TEXT("Layer_Monster"));

                    if (ptemp == nullptr) return;
                    static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));
                }

                for (int i = 0; i < 5; i++)
                {
                    int Random_pos = rand() % m_SpawnPos.size();
                    CGameObject* ptemp = nullptr;

                    ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Creeper"),
                        LEVEL_YU, TEXT("Layer_Monster"));

                    if (ptemp == nullptr) return;
                    static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));

                }

                int Random_pos = rand() % m_SpawnPos.size();
                CGameObject* ptemp = nullptr;

                ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Warden"),
                    LEVEL_YU, TEXT("Layer_Monster"));

                if (ptemp == nullptr) return;
                static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));

                static_cast<CCamera_Player*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Camera")))->Start_Cutscene(m_SpawnPos[Random_pos]);
                break;
            }
            }
        }
    }

    if (m_bIsLastWave && !m_bDayFinish) {
        if (m_sun->Get_Sun()) {
            m_sun->Set_bLastWave();
            Update_Mission(L"morning");
        }
        else {
            m_fSpwanCoolTime += fTimeDelta;
            if (m_fSpwanCoolTime >= 10.f)
            {
                int spawnCount = 20 - m_pGameInstance->GetActiveCount(TEXT("Layer_Monster"));
                if (spawnCount == 0) return;

                Spwan_Monster(spawnCount);
                m_fSpwanCoolTime = 0.f;
            }
        }
    }


    if (m_bNightFinish) {
        if (g_fBright >= 0.75) {
            m_sun->Set_bAddTime();
            m_bNightFinish = false;

            if (m_currentStage == STAGE_END)
            {
                m_bIsFireCracker = true;                
            }
        }
    }

}

void CMissionControl::Update(_float fTimeDelta)
{
    if (m_bIsFireCracker == true && m_fFireTime <= 0.f)
    {
        // ȭ���� ������ ���� ����D��....
        CFireCrackerLoad* obj = (CFireCrackerLoad*)m_pGameInstance->PushPool(LEVEL_YU,	// ������ ��
            PROTOTYPE_GAMEOBJECT_CRACKER_LOAD,	// ������ ������Ÿ��.
            LEVEL_YU,	// ���� ��.
            LAYER_PARTICLE);

        _float3 pos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION);

        pos.z += GetRandomFloat(-2.f, 2.f);
        pos.y += 2.f;
        pos.x += GetRandomFloat(-2.f, 2.f);

        obj->GetTransform()->Set_State(CTransform::STATE_POSITION, pos);

        m_fFireTime = m_fFireCoolTime;
    }

    // ���� ��Ÿ�� ����.
    if (m_bIsFireCracker == true)
    {
        m_fFireTime -= 1.f * fTimeDelta;
    }    

    bool m_checkFinishStage = true;
    switch (m_currentStage)
    {
    case DAY1:
        for (int i = 0; i < m_Round1.size(); ++i) {
            for (int j = 0; j < m_Round1[i].size(); ++j) {
                if (m_Round1[i][j].count >= m_Round1[i][j].finishCount) {
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
                if (m_Round1Wave[i][j].count >= m_Round1Wave[i][j].finishCount) {
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
                if (m_Round2[i][j].count >= m_Round2[i][j].finishCount) {
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
                if (m_Round2Wave[i][j].count >= m_Round2Wave[i][j].finishCount) {
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
                if (m_Round3[i][j].count >= m_Round3[i][j].finishCount) {
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
            m_bIsLastWave = true;
        }
        break;
    case WAVE3:
        for (int i = 0; i < m_Round3Wave.size(); ++i) {
            for (int j = 0; j < m_Round3Wave[i].size(); ++j) {
                if (m_Round3Wave[i][j].count >= m_Round3Wave[i][j].finishCount) {
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
            //m_bNightFinish = true;
            m_pGameInstance->PlayBGM(L"sweden");
            m_bIsWave = false;
            m_bIsLastWave = false;
            m_pGameInstance->ClearPool(L"Prototype_GameObject_Zombi");
            m_pGameInstance->ClearPool(L"Prototype_GameObject_Creeper");
        }
        break;
    default:
        break;
    }
}

void CMissionControl::Late_Update(_float fTimeDelta)
{

}

void CMissionControl::Spwan_Monster(int count)
{
    // ���� ���������� ��ġ ����
    m_SpawnPos = m_pTerrain->Get_SpwanAble();

    for (int i = 0; i < count; i++)
    {
        int Random_pos = rand() % m_SpawnPos.size();
        int Random_type = rand() % 2;
        CGameObject* ptemp = nullptr;

        if (Random_type == 0)
        {
            ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Creeper"),
                LEVEL_YU, TEXT("Layer_Monster"));
        }
        else
        {
            ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Zombi"),
                LEVEL_YU, TEXT("Layer_Monster"));
        }

        if (ptemp == nullptr) return;
        static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));
    }

}

void CMissionControl::Update_Mission(wstring name)
{
    switch (m_currentStage)
    {
    case DAY1:
        for (int i = 0; i < m_Round1.size(); ++i) {
            for (int j = 0; j < m_Round1[i].size(); ++j) {
                if (m_Round1[i][j].name == name) {
                    m_Round1[i][j].count++;
                }
            }
        }
        break;
    case WAVE1:
        for (int i = 0; i < m_Round1Wave.size(); ++i) {
            for (int j = 0; j < m_Round1Wave[i].size(); ++j) {
                if (m_Round1Wave[i][j].name == name) {
                    m_Round1Wave[i][j].count++;
                }
            }
        }
        break;
    case DAY2:
        for (int i = 0; i < m_Round2.size(); ++i) {
            for (int j = 0; j < m_Round2[i].size(); ++j) {
                if (m_Round2[i][j].name == name) {
                    m_Round2[i][j].count++;
                }
            }
        }
        break;
    case WAVE2:
        for (int i = 0; i < m_Round2Wave.size(); ++i) {
            for (int j = 0; j < m_Round2Wave[i].size(); ++j) {
                if (m_Round2Wave[i][j].name == name) {
                    m_Round2Wave[i][j].count++;
                }
            }
        }
        break;
    case DAY3:
        for (int i = 0; i < m_Round3.size(); ++i) {
            for (int j = 0; j < m_Round3[i].size(); ++j) {
                if (m_Round3[i][j].name == name) {
                    m_Round3[i][j].count++;
                }
            }
        }
        break;
    case WAVE3:
        for (int i = 0; i < m_Round3Wave.size(); ++i) {
            for (int j = 0; j < m_Round3Wave[i].size(); ++j) {
                if (m_Round3Wave[i][j].name == name) {
                    m_Round3Wave[i][j].count++;
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