#include "Key_Manager.h"

CKey_Manager::CKey_Manager()
{
    ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
    ZeroMemory(m_bPrevKeyState, sizeof(m_bPrevKeyState));
}

// Ű ���¸� �� ������ ������Ʈ
void CKey_Manager::Update()
{
    for (int i = 0; i < 256; ++i) // 256���� Ű�ڵ� üũ
    {
        m_bPrevKeyState[i] = m_bKeyState[i];  // ���� Ű ���� ����
        m_bKeyState[i] = (GetAsyncKeyState(i) & 0x8000) ? true : false;  // ���� Ű ���� ����
    }
}

// ���� Ű�� ��� ������ �ִ°�?
bool CKey_Manager::Key_Pressing(int _Key)
{
    return m_bKeyState[_Key];
}

// Ű�� ���ȴٰ� ������ ���� ����
bool CKey_Manager::Key_Up(int _Key)
{
    return m_bPrevKeyState[_Key] && !m_bKeyState[_Key];
}

// Ű�� ó�� ���� ���� ����
bool CKey_Manager::Key_Down(int _Key)
{
    return !m_bPrevKeyState[_Key] && m_bKeyState[_Key];
}

// Ű �Ŵ��� ����
CKey_Manager* CKey_Manager::Create()
{
    return new CKey_Manager();
}

// ���ҽ� ����
void CKey_Manager::Free()
{
}
