#pragma once
#include "Client_Defines.h"

BEGIN(Client)

// ������.
class CIObserver
{
protected:
	CIObserver() = default;
	virtual ~CIObserver() = default;

public:
	// Subject(���� ���� ���)���κ��� 
	// ����� ȣ���� ����.
	// �� �Լ��� �÷��̾� HP�� ����.
	virtual void OnNotifyPlayerHP(int _hp){}	
};

END