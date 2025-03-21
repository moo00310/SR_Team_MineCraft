#include "Sound_Manager.h"

CSound_Manager::CSound_Manager()
{
}

void CSound_Manager::Update()
{
    m_pStudioSystem->update();
    m_pCoreSystem->update();
    // ?œìˆœì¤‘ìš”??
}

FMOD::Studio::EventInstance* CSound_Manager::PlayEvent(const char* _EventPath)
{
    FMOD::Studio::EventDescription* pEventDescription; // ?´ë²¤?¸ì˜ ?•ë³´ë¥??€?¥í•˜???´ë˜??
    FMOD_RESULT result = m_pStudioSystem->getEvent(_EventPath, &pEventDescription); // ?œìŠ¤?œìœ¼ë¡?ë¶€???´ë¦„???µí•´ ?´ë²¤?¸ì˜ ?•ë³´ë¥?ë°›ì•„?¨ë‹¤.

    FMOD::Studio::EventInstance* pEventInstance = nullptr;
    pEventDescription->createInstance(&pEventInstance); // ?´ë²¤???¸ìŠ¤?´ìŠ¤ ?ì„±
    result = pEventInstance->start();
    pEventInstance->setCallback(AutoRelease, FMOD_STUDIO_EVENT_CALLBACK_STOPPED); // ?´ë²¤?¸ê? ?ë‚˜ë©?AutoRelease?¨ìˆ˜ë¥??µí•´ ?ë™?¼ë¡œ ë©”ëª¨ë¦¬ë? ë°˜í™˜?´ì???

    m_pStudioSystem->update();
    return pEventInstance;
}

FMOD::Studio::Bank* CSound_Manager::LoadBank(const char* _BankFilePath)
{
    FMOD::Studio::Bank* pBank = nullptr;
    FMOD_RESULT result = m_pStudioSystem->loadBankFile(_BankFilePath, FMOD_STUDIO_LOAD_BANK_NORMAL, &pBank);
    return pBank;
}

FMOD_RESULT F_CALLBACK AutoRelease(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE* eventInstance, void* parameters) // ?¬ìƒ???ë‚œ ?´ë²¤???ë™ ë°˜í™˜???„í•œ
{
    reinterpret_cast<FMOD::Studio::EventInstance*>(eventInstance)->release(); // FMOD ì½œë°±?¨ìˆ˜??C?€ C++??ë²”ìš©???¬ìš©???˜ê¸° ?„í•´ êµ¬ì¡°ì²??¬ì¸?°ë? ê¸°ë³¸ ë§¤ê°œë³€?˜ë¡œ ?¡ìŒ.
    //?´ë•Œë¬¸ì— C++?ì„œ??reinterpret_castë¥??µí•´ ?´ë˜???¬ì¸?°ë¡œ ?•ë????œì¼œì£¼ì–´?¼í•¨.
    return FMOD_OK;
}

void CSound_Manager::Stop_All_Event()
{
    FMOD::Studio::Bus* m_pBus = nullptr;
    m_pStudioSystem->getBus("bus:/", &m_pBus);
    m_pBus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE);
    Update();
}

HRESULT CSound_Manager::Initialize()
{
    FMOD::Studio::System::create(&m_pStudioSystem);
    m_pStudioSystem->getCoreSystem(&m_pCoreSystem);
    m_pStudioSystem->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);

    // ê¸°ë³¸?ìœ¼ë¡???ƒ ?„ìš”??Bank??ë¡œë“œ
    LoadBank("../../FMOD/Build/Desktop/Master.bank");
    LoadBank("../../FMOD/Build/Desktop/Master.strings.bank");
    //LoadBank("./examples/Build/Desktop/Music.bank");
    //LoadBank("./examples/Build/Desktop/SFX.bank");

    if (!m_pStudioSystem)
        return E_FAIL;

    return S_OK;
}
CSound_Manager* CSound_Manager::Create()
{
    CSound_Manager* pInstance = new CSound_Manager;

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CSoundManager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSound_Manager::Free()
{
    __super::Free();

    m_pStudioSystem->release();
    m_pCoreSystem->release();
}