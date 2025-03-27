#pragma once
#include "Client_Defines.h"
#include "IObserver.h"

BEGIN(Client)

// ��ü(���� ���� ���)
class CISubject
{
protected:
	CISubject() = default;
	virtual ~CISubject() = default;

public:
	// ������ �߰�.
	void AddObserver(CIObserver* _observer);

	// ������ ����.
	void RemoveObserver(CIObserver* _observer);

	// ��ϵ� �������鿡�� �÷��̾� HP ��ȣ�� ������ �뵵.
	void NotifyPlayerHP(int _hp);

private:
	// ��ϵ� ������.
	list<CIObserver*> m_ListObserver;
};

END