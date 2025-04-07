#include "Sound_Manager.h"
#include <windows.h>
#include <filesystem> // C++17 �ƴ�! �׳� ��� ���������θ� ��

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

    // �Ÿ��� 3 -> 25�� ������ �۾��� (�ƿ� �ȳ��� �ϴ� �� �ƴ�)
    pSound->set3DMinMaxDistance(3.0f, 25.0f);

    m_SoundMap[soundName] = pSound;

    return;
}

// ����� ���� ���
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

void CSound_Manager::PlaySound(const std::wstring& soundName, float volume, _float3 pPos, void* _obj, int _type)
{
    const float fListenerCutoffDistance = 25.0f; 

    // �ʹ� �־����� �Ҹ��� ������ ���� �Ÿ��̸� �Ҹ� �ƿ� play ���ϵ���
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

                iter = m_TrackedSounds.erase(iter); // ��� ���� �� ���� ��ҷ� �̵�
            }
            else
            {
                ++iter; // ������ �� ������ �׳� ��������
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

                iter = m_TrackedSounds.erase(iter); // ��� ���� �� ���� ��ҷ� �̵�
            }
            else
            {
                ++iter; // ������ �� ������ �׳� ��������
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

    // �� ���� ���� ���
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1,
        nullptr, 0, nullptr, nullptr);

    std::string result(sizeNeeded, 0);

    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1,
        &result[0], sizeNeeded, nullptr, nullptr);

    // ������ �ι��� ���� (���ڿ� ���� ����)
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
            LoadAllWavFiles(fullPath); // ��� Ž��
        }
        else {
            if (fullPath.size() >= 4 &&
                fullPath.substr(fullPath.size() - 4) == L".wav")
            {
                // ���� �̸�: Ȯ���� ����
                size_t lastSlash = fullPath.find_last_of(L"\\/");
                std::wstring fileName = (lastSlash != std::wstring::npos) ? fullPath.substr(lastSlash + 1) : fullPath;
                std::wstring soundNameW = fileName.substr(0, fileName.length() - 4);

                // ��ȯ: std::wstring �� std::string
                //std::string soundName = WStringToString(soundNameW);
                std::string filePath = WStringToString(fullPath);

                // ���� ���
                LoadSound(soundNameW.c_str(), filePath.c_str(), false);
            }
        }

    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
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