#include "Key_Manager.h"

CKey_Manager::CKey_Manager()
{
    ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
    ZeroMemory(m_bPrevKeyState, sizeof(m_bPrevKeyState));
}

// 키 상태를 매 프레임 업데이트
void CKey_Manager::Update()
{
    for (int i = 0; i < 256; ++i) // 256개의 키코드 체크
    {
        m_bPrevKeyState[i] = m_bKeyState[i];  // 이전 키 상태 저장
        m_bKeyState[i] = (GetAsyncKeyState(i) & 0x8000) ? true : false;  // 현재 키 상태 갱신
    }
}

// 현재 키가 계속 눌리고 있는가?
bool CKey_Manager::Key_Pressing(int _Key)
{
    return m_bKeyState[_Key];
}

// 키가 눌렸다가 떼어진 순간 감지
bool CKey_Manager::Key_Up(int _Key)
{
    return m_bPrevKeyState[_Key] && !m_bKeyState[_Key];
}

// 키가 처음 눌린 순간 감지
bool CKey_Manager::Key_Down(int _Key)
{
    return !m_bPrevKeyState[_Key] && m_bKeyState[_Key];
}

// 키 매니저 생성
CKey_Manager* CKey_Manager::Create()
{
    return new CKey_Manager();
}

// 리소스 정리
void CKey_Manager::Free()
{
}
