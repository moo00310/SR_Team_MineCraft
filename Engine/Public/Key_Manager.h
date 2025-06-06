#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKey_Manager final : public CBase
{
private:
    CKey_Manager();
    virtual ~CKey_Manager() = default;

public:
    void Update();   // 한 프레임마다 키 상태 업데이트
    bool Key_Pressing(int _Key);
    bool Key_Up(int _Key);   // 누르고 있다가 뗐을 때
    bool Key_Down(int _Key); // 눌렀을 때

private:
    bool m_bKeyState[VK_MAX];      // 현재 프레임 키 상태
    bool m_bPrevKeyState[VK_MAX];  // 이전 프레임 키 상태

public:
    static CKey_Manager* Create();
    virtual void Free() override;
};

END
