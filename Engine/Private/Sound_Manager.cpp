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

    auto iter = m_SoundMap.find(soundName);
    if (iter != m_SoundMap.end())
        return;


    FMOD_MODE mode = FMOD_DEFAULT;
    if (bLoop)
        mode |= FMOD_LOOP_NORMAL;
    else
        mode |= FMOD_LOOP_OFF;

    mode |= FMOD_3D;
    mode |= FMOD_3D_LINEARROLLOFF;

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

    m_pCoreSystem->playSound(pSound, nullptr, false, &m_pBGMChannel);
}

void CSound_Manager::PlaySound(const std::wstring& soundName, float volume, _float3 pPos, void* _obj )
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
                TrackedSound _sound = { m_pChannels[i], _obj };
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

void CSound_Manager::CheckCreeperExplosion(void* obj, int _anim)
{
    for (auto iter = m_TrackedSounds.begin(); iter != m_TrackedSounds.end(); )
    {
        if (iter->pObj == obj && _anim != 3)
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
}

void CSound_Manager::Add_SoundResource()
{
    LoadSound(L"Player_Walk_Grass1", "../../FMOD/Assets/player/Player_Walk_Grass1.wav", false);
    LoadSound(L"Player_Walk_Grass2", "../../FMOD/Assets/player/Player_Walk_Grass2.wav", false);
    LoadSound(L"Player_Walk_Wood1", "../../FMOD/Assets/player/Player_Walk_Wood1.wav", false);
    LoadSound(L"Player_Walk_Wood2", "../../FMOD/Assets/player/Player_Walk_Wood2.wav", false);
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