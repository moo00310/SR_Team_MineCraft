#include "Sound_Manager.h"

CSound_Manager::CSound_Manager()
{
}

void CSound_Manager::Update()
{
    m_pStudioSystem->update();
    m_pCoreSystem->update();
    // ?�순중요??
}

FMOD::Studio::EventInstance* CSound_Manager::PlayEvent(const char* _EventPath)
{
    FMOD::Studio::EventDescription* pEventDescription; // ?�벤?�의 ?�보�??�?�하???�래??
    FMOD_RESULT result = m_pStudioSystem->getEvent(_EventPath, &pEventDescription); // ?�스?�으�?부???�름???�해 ?�벤?�의 ?�보�?받아?�다.

    FMOD::Studio::EventInstance* pEventInstance = nullptr;
    pEventDescription->createInstance(&pEventInstance); // ?�벤???�스?�스 ?�성
    result = pEventInstance->start();
    pEventInstance->setCallback(AutoRelease, FMOD_STUDIO_EVENT_CALLBACK_STOPPED); // ?�벤?��? ?�나�?AutoRelease?�수�??�해 ?�동?�로 메모리�? 반환?��???

    m_pStudioSystem->update();
    return pEventInstance;
}

FMOD::Studio::Bank* CSound_Manager::LoadBank(const char* _BankFilePath)
{
    FMOD::Studio::Bank* pBank = nullptr;
    FMOD_RESULT result = m_pStudioSystem->loadBankFile(_BankFilePath, FMOD_STUDIO_LOAD_BANK_NORMAL, &pBank);
    return pBank;
}

FMOD_RESULT F_CALLBACK AutoRelease(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE* eventInstance, void* parameters) // ?�생???�난 ?�벤???�동 반환???�한
{
    reinterpret_cast<FMOD::Studio::EventInstance*>(eventInstance)->release(); // FMOD 콜백?�수??C?� C++??범용???�용???�기 ?�해 구조�??�인?��? 기본 매개변?�로 ?�음.
    //?�때문에 C++?�서??reinterpret_cast�??�해 ?�래???�인?�로 ?��????�켜주어?�함.
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

    // 기본?�으�???�� ?�요??Bank??로드
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