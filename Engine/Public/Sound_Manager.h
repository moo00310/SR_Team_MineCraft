#pragma once
#include "fmod.hpp"
#include "fmod_studio.hpp"

// Debug 일 때
#pragma comment(lib, "fmodL_vc.lib")
#pragma comment(lib, "fmodstudioL_vc.lib")

// Release 일 때
#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodstudio_vc.lib")

#include "Base.h"

#define MAX_SFX_CHANNEL 31
FMOD_RESULT F_CALLBACK AutoRelease(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE* eventInstance, void* parameters);

BEGIN(Engine)

class CSound_Manager : public CBase
{
private:
    CSound_Manager();
    virtual ~CSound_Manager() = default;

public:
    void Update();

public:
    FMOD::Studio::EventInstance* PlayEvent(const char* _EventPath);
    FMOD::Studio::Bank* LoadBank(const char* _BankFilePath);
    void Stop_All_Event();

private:
    FMOD::System* m_pCoreSystem{ nullptr };
    FMOD::Studio::System* m_pStudioSystem{ nullptr };

public:
    HRESULT						Initialize();
    static CSound_Manager*       Create();
    virtual void				Free() override;
};

END