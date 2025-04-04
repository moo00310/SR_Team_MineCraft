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
    // 파일 로드 하는 곳 init에서 모두 로드할 예정
    // bgm은 loop true 시켜주면 됨
    void LoadSound(const std::wstring& soundName, const char* filePath, bool loop = false);
    // bgm 재생
    void PlayBGM(const std::wstring& soundName);
    // 소리 재생
    void PlaySound(const std::wstring& soundName, float volume = 1.0f, _float3 pPos = _float3(0.f, 0.f, 0.f));
    // 이펙트 소리와 bgm 소리 모두 끔
    void StopAll();
    // 플레이어 위치 업데이트
    void UpdateListener(_float3 pos, _float3 forward, _float3 up);

private:
    void Add_SoundResource();

    FMOD::System* m_pCoreSystem{ nullptr };
    FMOD::Channel* m_pChannels[MAX_SFX_CHANNEL]{ nullptr };
    FMOD::Channel* m_pBGMChannel;

    std::map<std::wstring, FMOD::Sound*> m_SoundMap;
    _float3 m_vListenerPos;
public:
    static CSound_Manager* Create();
    virtual void Free() override;
};

END