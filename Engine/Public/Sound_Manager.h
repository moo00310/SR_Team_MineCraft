#pragma once
#include "fmod.hpp"

#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif

#include "Base.h"

#define MAX_SFX_CHANNEL 32

BEGIN(Engine)

class CSound_Manager : public CBase
{
private:
    CSound_Manager();
    virtual ~CSound_Manager() = default;

public:
    HRESULT Initialize();
    void Update();
    void LoadSound(const std::wstring& soundName, const char* filePath, bool loop = false);
    void PlayBGM(const std::wstring& soundName);
    void PlaySound(const std::wstring& soundName, float volume = 1.0f, const _float3& pPos = _float3(0.f, 0.f, 0.f));
    void StopAll();
    void UpdateListener(const _float3& pos, const _float3& forward, const _float3& up);

private:
    void Add_SoundResource();

    FMOD::System* m_pCoreSystem{ nullptr };
    FMOD::Channel* m_pChannels[MAX_SFX_CHANNEL]{ nullptr };
    FMOD::Channel* m_pBGMChannel;

    std::map<std::wstring, FMOD::Sound*> m_SoundMap;
public:
    static CSound_Manager* Create();
    virtual void Free() override;
};

END