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
public:
    struct TrackedSound {
        FMOD::Channel* pChannel = nullptr;
        void* pObj = nullptr;
    };
private:
    CSound_Manager();
    virtual ~CSound_Manager() = default;

public:
    HRESULT Initialize();
    void Update();
    // ���� �ε� �ϴ� �� init���� ��� �ε��� ����
    // bgm�� loop true �����ָ� ��
    void LoadSound(const std::wstring& soundName, const char* filePath, bool loop = false);
    // bgm ���
    void PlayBGM(const std::wstring& soundName);
    // �Ҹ� ���
    void PlaySound(const std::wstring& soundName, float volume = 1.0f, _float3 pPos = _float3(0.f, 0.f, 0.f), void* _obj = nullptr);
    // ����Ʈ �Ҹ��� bgm �Ҹ� ��� ��
    void StopAll();
    // �÷��̾� ��ġ ������Ʈ
    void UpdateListener(_float3 pos, _float3 forward, _float3 up);

    // ũ���� ������ �ִ� ��
    void CheckCreeperExplosion(void* obj, int _anim);

private:
    void Add_SoundResource();

    FMOD::System* m_pCoreSystem{ nullptr };
    FMOD::Channel* m_pChannels[MAX_SFX_CHANNEL]{ nullptr };
    FMOD::Channel* m_pBGMChannel;

    std::map<std::wstring, FMOD::Sound*> m_SoundMap;
    _float3 m_vListenerPos;
    std::vector<TrackedSound> m_TrackedSounds;

public:
    static CSound_Manager* Create();
    virtual void Free() override;
};

END