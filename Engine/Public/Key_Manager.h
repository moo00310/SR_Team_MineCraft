#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKey_Manager final : public CBase
{
private:
    CKey_Manager();
    virtual ~CKey_Manager() = default;

public:
    void Update();   // �� �����Ӹ��� Ű ���� ������Ʈ
    bool Key_Pressing(int _Key);
    bool Key_Up(int _Key);   // ������ �ִٰ� ���� ��
    bool Key_Down(int _Key); // ������ ��

private:
    bool m_bKeyState[VK_MAX];      // ���� ������ Ű ����
    bool m_bPrevKeyState[VK_MAX];  // ���� ������ Ű ����

public:
    static CKey_Manager* Create();
    virtual void Free() override;
};

END
