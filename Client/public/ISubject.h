#pragma once
#include "Client_Defines.h"
#include "IObserver.h"

BEGIN(Client)

// 주체(관찰 받을 대상)
class CISubject
{
protected:
	CISubject() = default;
	virtual ~CISubject() = default;

public:
	// 옵저버 추가.
	void AddObserver(CIObserver* _observer);

	// 옵저버 제거.
	void RemoveObserver(CIObserver* _observer);

	// 등록된 옵저버들에게 플레이어 HP 신호를 보내는 용도.
	void NotifyPlayerHP(int _hp);

private:
	// 등록된 옵저버.
	list<CIObserver*> m_ListObserver;
};

END