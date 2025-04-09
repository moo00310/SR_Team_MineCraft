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


void CSound_Manager::Play_Sound(const wstring& soundName, _uint iType, CGameObject* pGameObject, _float volume, _float3 pPos)
{
    if (!m_pCoreSystem || !pGameObject)
        return;

    auto iter = m_SoundMap.find(soundName);
    if (iter == m_SoundMap.end())
        return;

    FMOD::Sound* pSound = iter->second;
    if (!pSound)
        return;

    const float fListenerCutoffDistance = 25.0f;
    _float3 vDiff{ pPos - m_vListenerPos };
    float fNewDistance = D3DXVec3Length(&vDiff);
    if (fNewDistance > fListenerCutoffDistance)
        return;
#pragma region
    ////이미 같은 오브젝트 + 타입으로 재생 중인지 확인
    //auto& typeMap = m_ObjectChannelMap[pGameObject];
    //auto itChannel = typeMap.find(iType);



    //if (itChannel != typeMap.end())
    //{
    //    bool isPlaying = false;
    //    if (itChannel->second && itChannel->second->isPlaying(&isPlaying) == FMOD_OK && isPlaying)
    //    {
    //        return; // 이미 소리 재생 중 → 무시
    //    }
    //    else
    //    {
    //        // 죽은 채널 정리
    //        typeMap.erase(itChannel);
    //    }
    //}
#pragma endregion

    // 새 채널 재생
    FMOD::Channel* pNewChannel = nullptr;
    if (m_pCoreSystem->playSound(pSound, nullptr, false, &pNewChannel) == FMOD_OK && pNewChannel)
    {
        pNewChannel->setVolume(volume);

        FMOD_VECTOR pos = { pPos.x, pPos.y, pPos.z };
        FMOD_VECTOR vel = { 0.f, 0.f, 0.f };
        pNewChannel->set3DAttributes(&pos, &vel);

        // 등록
        m_ObjectChannelMap[pGameObject][iType] = pNewChannel;
    }
}



void CSound_Manager::StopAll()
{
   /* for (int i = 0; i < MAX_SFX_CHANNEL; ++i)
    {
        if (m_pChannels[i])
            m_pChannels[i]->stop();
    }

    m_pBGMChannel->stop();*/
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

_bool CSound_Manager::IsPlaying_Sound(_int iType)
{
    _bool isPlaying = false;

    //m_pChannels[iType]->isPlaying(&isPlaying);

    return isPlaying;
}

void CSound_Manager::Stop_Sound(_int iType, CGameObject* pObject)
{
    if (!pObject)
        return;

    auto objIt = m_ObjectChannelMap.find(pObject);
    if (objIt == m_ObjectChannelMap.end())
        return;

    auto& typeMap = objIt->second;
    auto chIt = typeMap.find(iType);
    if (chIt == typeMap.end())
        return;

    if (chIt->second)
    {
        chIt->second->stop(); //사운드 정지
    }

    typeMap.erase(chIt); // 채널 제거

    // 해당 오브젝트의 사운드가 전부 제거됐으면 map도 제거
    if (typeMap.empty())
        m_ObjectChannelMap.erase(objIt);
}


//_bool CSound_Manager::Check_Sound_Play(void* obj, int _type)
//{
//    _bool isPlay = false;
//
//    for (auto iter = m_TrackedSounds.begin(); iter != m_TrackedSounds.end(); )
//    {
//        if (iter->pObj == obj && iter->iType == _type)
//        {
//            if (iter->pChannel)
//                iter->pChannel->isPlaying(&isPlay);
//
//            iter = m_TrackedSounds.erase(iter);
//            isPlay = true;
//        }
//        else
//        {
//            ++iter;
//        }
//    }
//
//    return isPlay;
//
//    //_int soundType = _type;
//    //switch (soundType)
//    //{
//    //case Engine::CSound_Manager::CREEPER:
//    //    for (auto iter = m_TrackedSounds.begin(); iter != m_TrackedSounds.end(); )
//    //    {
//    //        if (iter->pObj == obj && _anim != 3 && iter->iType == 0)
//    //        {
//    //            if (iter->pChannel)
//    //                iter->pChannel->stop();
//
//    //            iter = m_TrackedSounds.erase(iter); // 요소 제거 후 다음 요소로 이동
//    //        }
//    //        else
//    //        {
//    //            ++iter; // 조건이 안 맞으면 그냥 다음으로
//    //        }
//    //    }
//    //    break;
//    //case Engine::CSound_Manager::BREAKING:
//    //    for (auto iter = m_TrackedSounds.begin(); iter != m_TrackedSounds.end(); )
//    //    {
//    //        if (iter->iType == 1)
//    //        {
//    //            if (iter->pChannel)
//    //                iter->pChannel->stop();
//
//    //            iter = m_TrackedSounds.erase(iter); // 요소 제거 후 다음 요소로 이동
//    //        }
//    //        else
//    //        {
//    //            ++iter; // 조건이 안 맞으면 그냥 다음으로
//    //        }
//    //    }
//    //    break;
//    //case Engine::CSound_Manager::SOUND_END:
//    //    break;
//    //default:
//    //    break;
//    //}
//
//}

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
                //background 경로 걸러내기
                std::wstring lowerPath = fullPath;
                std::transform(lowerPath.begin(), lowerPath.end(), lowerPath.begin(), ::towlower);
                if (lowerPath.find(L"\\background\\") != std::wstring::npos ||
                    lowerPath.find(L"/background/") != std::wstring::npos)
                {
                    continue;
                }

                // 사운드 이름: 확장자 제거
                size_t lastSlash = fullPath.find_last_of(L"\\/");
                std::wstring fileName = (lastSlash != std::wstring::npos) ? fullPath.substr(lastSlash + 1) : fullPath;
                std::wstring soundNameW = fileName.substr(0, fileName.length() - 4);

                std::string filePath = WStringToString(fullPath);

                LoadSound(soundNameW.c_str(), filePath.c_str(), false);
            }
        }

    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);

    // 배경음악은 수동 등록
    LoadSound(L"MoogCity2", "../../FMOD/Assets/background/MoogCity2.wav", true);
    LoadSound(L"Beginning", "../../FMOD/Assets/background/Beginning.wav", true);
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