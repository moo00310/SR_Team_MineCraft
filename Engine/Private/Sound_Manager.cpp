#include "Sound_Manager.h"

#include <windows.h>
#include <filesystem> // C++17 아님! 그냥 경로 가공용으로만 씀

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

    auto iter = m_SoundMap.find(soundName);
    if (iter != m_SoundMap.end())
        return;


    FMOD_MODE mode = FMOD_DEFAULT;
    if (bLoop) {
        mode |= FMOD_LOOP_NORMAL;
        mode |= FMOD_2D;
    }
    else {
        mode |= FMOD_LOOP_OFF;
        mode |= FMOD_3D;
        mode |= FMOD_3D_LINEARROLLOFF;
    }
        

    FMOD::Sound* pSound = nullptr;
    if (m_pCoreSystem->createSound(filePath, mode, nullptr, &pSound) != FMOD_OK)
    {
        return;
    }

    // 거리가 3 -> 25로 갈수록 작아짐 (아예 안나게 하는 건 아님)
    pSound->set3DMinMaxDistance(3.0f, 25.0f);

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

    bool isPlaying = false;
    if (m_pBGMChannel)
    {
        m_pBGMChannel->isPlaying(&isPlaying);
        if (isPlaying)
        {
            m_pBGMChannel->stop();
        }
    }

    m_pCoreSystem->playSound(pSound, nullptr, false, &m_pBGMChannel);
}


void CSound_Manager::PlaySound(const std::wstring& soundName, float volume, _float3 pPos, void* _obj, int _type)
{
    const float fListenerCutoffDistance = 25.0f; 

    // 너무 멀어지면 소리가 깨져서 일정 거리이면 소리 아예 play 안하도록
    _float3 listenerPos = m_vListenerPos; 
    _float3 Diff = pPos - listenerPos;
    float fDistance = D3DXVec3Length(&Diff);
    if (fDistance > fListenerCutoffDistance)
        return; 


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

            if (_obj) {
                TrackedSound _sound = { m_pChannels[i], _obj, static_cast<SEPCIALSOUNDTYPE>(_type) };
                m_TrackedSounds.push_back(_sound);
            }

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

void CSound_Manager::UpdateListener(_float3 _pos, _float3 _forward, _float3 _up)
{
    FMOD_VECTOR pos = { _pos.x, _pos.y, _pos.z };
    FMOD_VECTOR forward = { _forward.x, _forward.y, _forward.z };
    FMOD_VECTOR up = { _up.x, _up.y, _up.z };

    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
    m_pCoreSystem->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
    m_vListenerPos = _pos;
}

void CSound_Manager::CheckSoundStop(void* obj, int _anim, int _type)
{
    SEPCIALSOUNDTYPE soundType = static_cast<SEPCIALSOUNDTYPE>(_type);
    switch (soundType)
    {
    case Engine::CSound_Manager::CREEPER:
        for (auto iter = m_TrackedSounds.begin(); iter != m_TrackedSounds.end(); )
        {
            if (iter->pObj == obj && _anim != 3 && iter->type == 0)
            {
                if (iter->pChannel)
                    iter->pChannel->stop();

                iter = m_TrackedSounds.erase(iter); // 요소 제거 후 다음 요소로 이동
            }
            else
            {
                ++iter; // 조건이 안 맞으면 그냥 다음으로
            }
        }
        break;
    case Engine::CSound_Manager::BREAKING:
        for (auto iter = m_TrackedSounds.begin(); iter != m_TrackedSounds.end(); )
        {
            if (iter->type == 1)
            {
                if (iter->pChannel)
                    iter->pChannel->stop();

                iter = m_TrackedSounds.erase(iter); // 요소 제거 후 다음 요소로 이동
            }
            else
            {
                ++iter; // 조건이 안 맞으면 그냥 다음으로
            }
        }
        break;
    case Engine::CSound_Manager::SOUND_END:
        break;
    default:
        break;
    }

}

string CSound_Manager::WStringToString(const std::wstring& wstr)
{
    if (wstr.empty())
        return std::string();

    // 널 포함 길이 계산
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1,
        nullptr, 0, nullptr, nullptr);

    std::string result(sizeNeeded, 0);

    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1,
        &result[0], sizeNeeded, nullptr, nullptr);

    // 마지막 널문자 제거 (문자열 길이 조정)
    if (!result.empty() && result.back() == '\0')
        result.pop_back();

    return result;
}

void CSound_Manager::Add_SoundResource()
{
    LoadAllWavFiles(L"../../FMOD/Assets");
}

void CSound_Manager::LoadAllWavFiles(const std::wstring& folderPath)
{
    std::wstring searchPath = folderPath + L"\\*";

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do {
        const std::wstring name = findData.cFileName;

        if (name == L"." || name == L"..")
            continue;

        std::wstring fullPath = folderPath + L"\\" + name;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            LoadAllWavFiles(fullPath); // 재귀 탐색
        }
        else {
            if (fullPath.size() >= 4 &&
                fullPath.substr(fullPath.size() - 4) == L".wav")
            {
                // 사운드 이름: 확장자 제거
                size_t lastSlash = fullPath.find_last_of(L"\\/");
                std::wstring fileName = (lastSlash != std::wstring::npos) ? fullPath.substr(lastSlash + 1) : fullPath;
                std::wstring soundNameW = fileName.substr(0, fileName.length() - 4);

                // 변환: std::wstring → std::string
                //std::string soundName = WStringToString(soundNameW);
                std::string filePath = WStringToString(fullPath);

                // 사운드 등록
                LoadSound(soundNameW.c_str(), filePath.c_str(), false);
            }
        }

    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);

    LoadSound(L"Player_Walk_Grass1", "../../FMOD/Assets/player/Player_Walk_Grass1.wav", false);
    LoadSound(L"Player_Walk_Grass2", "../../FMOD/Assets/player/Player_Walk_Grass2.wav", false);
    LoadSound(L"Player_Walk_Grass3", "../../FMOD/Assets/player/Player_Walk_Grass2.wav", false);
    LoadSound(L"Player_Walk_Grass4", "../../FMOD/Assets/player/Player_Walk_Grass2.wav", false);
    LoadSound(L"Player_Hurt", "../../FMOD/Assets/player/Player_Hurt.wav", false);
    LoadSound(L"Player_Eat1", "../../FMOD/Assets/player/Player_Eat1.wav", false);
    LoadSound(L"Player_Eat2", "../../FMOD/Assets/player/Player_Eat2.wav", false);
    LoadSound(L"Player_Eat3", "../../FMOD/Assets/player/Player_Eat3.wav", false);

    LoadSound(L"Zombie_Walk1", "../../FMOD/Assets/zombie/Zombie_Walk1.wav", false);
    LoadSound(L"Zombie_Walk2", "../../FMOD/Assets/zombie/Zombie_Walk2.wav", false);
    LoadSound(L"Zombie_Say1", "../../FMOD/Assets/zombie/Zombie_Say1.wav", false);
    LoadSound(L"Zombie_Say2", "../../FMOD/Assets/zombie/Zombie_Say2.wav", false);
    LoadSound(L"Zombie_Say3", "../../FMOD/Assets/zombie/Zombie_Say3.wav", false);
    LoadSound(L"Zombie_Hurt1", "../../FMOD/Assets/zombie/Zombie_Hurt1.wav", false);
    LoadSound(L"Zombie_Hurt2", "../../FMOD/Assets/zombie/Zombie_Hurt2.wav", false);
    LoadSound(L"Zombie_Death", "../../FMOD/Assets/zombie/Zombie_Death.wav", false);

    LoadSound(L"Creeper_Hurt1", "../../FMOD/Assets/creeper/Creeper_Hurt1.wav", false);
    LoadSound(L"Creeper_Hurt2", "../../FMOD/Assets/creeper/Creeper_Hurt2.wav", false);
    LoadSound(L"Creeper_Death", "../../FMOD/Assets/creeper/Creeper_Death.wav", false);
    LoadSound(L"Creeper_Explosion", "../../FMOD/Assets/creeper/Creeper_Explosion.wav", false);

    LoadSound(L"Block_Breaking", "../../FMOD/Assets/Block/Block_Breaking.wav", false);
    LoadSound(L"Block_BreakingFinish", "../../FMOD/Assets/Block/Block_BreakingFinish.wav", false);


    LoadSound(L"MoogCity2", "../../FMOD/Assets/background/MoogCity2.wav", true);
    LoadSound(L"pigstep", "../../FMOD/Assets/background/pigstep.wav", true);
    LoadSound(L"sweden", "../../FMOD/Assets/background/sweden.wav", true);

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