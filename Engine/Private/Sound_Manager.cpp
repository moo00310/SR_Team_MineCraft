#include "Sound_Manager.h"

CSound_Manager::CSound_Manager()
{
}

HRESULT CSound_Manager::Initialize()
{
    FMOD_RESULT result = FMOD::System_Create(&m_pCoreSystem);
    if (result != FMOD_OK) return E_FAIL;

    result = m_pCoreSystem->init(MAX_SFX_CHANNEL, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) return E_FAIL;

    Add_SoundResource();

    return S_OK;
}

void CSound_Manager::LoadSound(const std::wstring& soundName, const char* filePath, bool bLoop)
{
    if (!m_pCoreSystem)
        return;

    // 이미 등록된 사운드면 리턴
    auto iter = m_SoundMap.find(soundName);
    if (iter != m_SoundMap.end())
        return;


    FMOD_MODE mode = FMOD_DEFAULT;
    if (bLoop)
        mode |= FMOD_LOOP_NORMAL;
    else
        mode |= FMOD_LOOP_OFF;

    mode |= FMOD_3D;

    FMOD::Sound* pSound = nullptr;
    if (m_pCoreSystem->createSound(filePath, mode, nullptr, &pSound) != FMOD_OK)
    {
        return;
    }

    // 맵에 저장
    m_SoundMap[soundName] = pSound;

    return;
}

// 배경음 전용 재생
void CSound_Manager::PlayBGM(const std::wstring& soundName)
{
    FMOD::Sound* pSound = nullptr;
    auto iter = m_SoundMap.find(soundName);
    if (iter != m_SoundMap.end())
        pSound = iter->second;

    if (pSound == nullptr)
        return;

    m_pCoreSystem->playSound(pSound, nullptr, false, &m_pBGMChannel);
}

void CSound_Manager::PlaySound(const std::wstring& soundName, float volume, const _float3& pPos)
{
    FMOD::Sound* pSound = nullptr;
    auto iter = m_SoundMap.find(soundName);
     if (iter != m_SoundMap.end())
         pSound = iter->second;

    if (!pSound || !m_pCoreSystem) return;

    for (int i = 0; i < MAX_SFX_CHANNEL; ++i)
    {
        bool isPlaying = false;
        if (m_pChannels[i])
            m_pChannels[i]->isPlaying(&isPlaying);

        if (!isPlaying)
        {
            m_pCoreSystem->playSound(pSound, nullptr, false, &m_pChannels[i]);
            m_pChannels[i]->setVolume(volume);

            FMOD_VECTOR pos = { pPos.x, pPos.y, pPos.z };
            FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
            m_pChannels[i]->set3DAttributes(&pos, &vel);

            break;
        }
    }
}

void CSound_Manager::StopAll()
{
    for (int i = 0; i < MAX_SFX_CHANNEL; ++i)
    {
        if (m_pChannels[i])
            m_pChannels[i]->stop();
    }

    m_pBGMChannel->stop();
}

void CSound_Manager::UpdateListener(const _float3& _pos, const _float3& _forward, const _float3& _up)
{
    FMOD_VECTOR pos = { _pos.x, _pos.y, _pos.z };
    FMOD_VECTOR forward = { _forward.x, _forward.y, _forward.z };
    FMOD_VECTOR up = { _up.x, _up.y, _up.z };

    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
    m_pCoreSystem->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
}

void CSound_Manager::Add_SoundResource()
{
    LoadSound(L"Player_Jump", "../../FMOD/Assets/Built_Fail.wav", false);
}

void CSound_Manager::Update()
{
    if (m_pCoreSystem)
        m_pCoreSystem->update();
}

CSound_Manager* CSound_Manager::Create()
{
    CSound_Manager* pInstance = new CSound_Manager();

    if (FAILED(pInstance->Initialize()))
    {
        delete pInstance;
        return nullptr;
    }

    return pInstance;
}

void CSound_Manager::Free()
{
    StopAll();

    for (auto& soundPair : m_SoundMap)
    {
        if (soundPair.second)
            soundPair.second->release();
    }
    m_SoundMap.clear();

    if (m_pCoreSystem)
    {
        m_pCoreSystem->close();
        m_pCoreSystem->release();
        m_pCoreSystem = nullptr;
    }
}