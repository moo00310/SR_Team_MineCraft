#pragma once
#include "Client_Defines.h"

BEGIN(Client)

// 관찰자.
class CIObserver
{
protected:
	CIObserver() = default;
	virtual ~CIObserver() = default;

public:
	// Subject(관찰 받을 대상)으로부터 
	// 변경된 호출을 받음.
	// 이 함수는 플레이어 HP를 받음.
	virtual void OnNotifyPlayerHP(int _hp){}	
};

END