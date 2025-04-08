#pragma once
#include "fmod.hpp"

#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif

#include "Base.h"
#include "GameObject.h"

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
    // ���� �ε� �ϴ� �� init���� ��� �ε��� ����
    // bgm�� loop true �����ָ� ��
    void LoadSound(const wstring& soundName, const char* filePath, bool loop = false);
    // bgm ���
    void PlayBGM(const wstring& soundName);
    // �Ҹ� ���
    void Play_Sound(const wstring& soundName, _uint iType, CGameObject* pGameObject, _float volume = 1.0f, _float3 pPos = _float3{ 0.f, 0.f, 0.f });
    // ����Ʈ �Ҹ��� bgm �Ҹ� ��� ��
    void StopAll();
    // �÷��̾� ��ġ ������Ʈ
    void UpdateListener(_float3 pos, _float3 forward, _float3 up);

    //���尡 �÷��� ���ΰ�?
    _bool   IsPlaying_Sound(_int iType);
    //���� �����
    void    Stop_Sound(_int iType, CGameObject* pObject);

private:
    void Add_SoundResource();
    void LoadAllWavFiles(const std::wstring& folderPath);
    string WStringToString(const std::wstring& wstr);

    FMOD::System* m_pCoreSystem{ nullptr };
    // map<���� Ÿ��, ä�� ���>
    std::map<_int, std::vector<FMOD::Channel*>> m_ChannelMap;

    FMOD::Channel* m_pBGMChannel;
    std::map<std::wstring, FMOD::Sound*> m_SoundMap;
    _float3 m_vListenerPos;

    // [������Ʈ][���� Ÿ��] �� ä�� ������
    std::unordered_map<CGameObject*, std::unordered_map<_uint, FMOD::Channel*>> m_ObjectChannelMap;


public:
    static CSound_Manager* Create();
    virtual void Free() override;
};

END