#include "ISubject.h"

void CISubject::AddObserver(CIObserver* _observer)
{
	m_ListObserver.push_back(_observer);
}

void CISubject::RemoveObserver(CIObserver* _observer)
{
	m_ListObserver.remove(_observer);
}

void CISubject::NotifyPlayerHP(int _hp)
{
	for (auto observer : m_ListObserver)
	{
		observer->OnNotifyPlayerHP(_hp);
	}
}
