#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include <algorithm>
#include <iostream>

BEGIN(Client)
enum class STATUS { NOT_STARTED, SUCCESS, FAIL, RUNNING, STATUS_END };

//Node
class CNode : public CBase
{
protected:
	virtual ~CNode() = default;
public://CGameObject* _Obj를 전달할 필요 없이 초기화 때 받는게 나을 것 같음
	virtual STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) = 0;
public:
	virtual void Free() override {};
};

//CompositeNode
class CCompositeNode abstract : public CNode
{
protected:
	virtual ~CCompositeNode() = default;
	int m_iRunningIndex = -1;
public:
	vector<CNode*> Get_VecNodes(void) { return m_vecNodes; }
	void Add_Node(CNode* _pNode) { m_vecNodes.push_back(_pNode); }

protected:
	vector<CNode*> Get_ShuffeldNodes(void) const {
		vector<CNode*> vecTemp = m_vecNodes;
		random_shuffle(vecTemp.begin(), vecTemp.end());
		return vecTemp;
	}
protected:
	vector<CNode*> m_vecNodes;
public:
	virtual void Free() override
	{
		for (auto& iter : m_vecNodes)
		{
			Safe_Release(iter);
			iter = nullptr;
		}
		m_vecNodes.clear();
	}
};

//SequenceNode
class CSequenceNode : public CCompositeNode
{
protected:
	wstring Sequence_Name;
	virtual ~CSequenceNode() = default;
public:
	CSequenceNode(wstring _wsName) { Sequence_Name = _wsName; }

	virtual STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override
	{
		for (CNode* iter : Get_VecNodes())
		{
			STATUS eResult = iter->Excute(_Obj, _fTimeDelta);

			if (eResult == STATUS::FAIL)
			{
				return STATUS::FAIL;
			}
			else if (eResult == STATUS::RUNNING)
			{
				return STATUS::RUNNING;
			}
		}
		return STATUS::SUCCESS;
	}

public:
	virtual void Free() override
	{
		__super::Free();
	}
};

//Selector
class CSelectorNode : public CCompositeNode
{
protected:
	wstring Selector_Name;

	virtual ~CSelectorNode() = default;
public:
	CSelectorNode(wstring _wsName) { Selector_Name = _wsName; }

	virtual STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override
	{
		for (CNode* iter : Get_VecNodes())
		{

			STATUS eResult = iter->Excute(_Obj, _fTimeDelta);
			if (eResult == STATUS::SUCCESS)
			{
				return  STATUS::SUCCESS;
			}
			else if (eResult == STATUS::RUNNING)
			{
				return STATUS::RUNNING;
			}
		}
		return STATUS::FAIL;
	}
public:
	virtual void Free() override
	{
		__super::Free();
	}
};

//RandomSelector
class CRandomSelector : public CCompositeNode
{
protected:
	virtual ~CRandomSelector() = default;

private:
	CNode* m_pCurrent = nullptr;

public:
	virtual STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) override
	{
		if (m_pCurrent == nullptr)
		{
			auto shuffled = Get_ShuffeldNodes();
			m_pCurrent = shuffled[0];  // 하나만 선택
		}

		STATUS eResult = m_pCurrent->Excute(_Obj, _fTimeDelta);

		if (eResult == STATUS::SUCCESS || eResult == STATUS::FAIL)
		{
			m_pCurrent = nullptr;  // 초기화
		}

		return eResult;
	}
	
public:
	virtual void Free() override
	{
		__super::Free();
	}
};

//Decorator_If
class CDecorator_If : public CCompositeNode
{
protected:
	virtual ~CDecorator_If() = default;
public:
	virtual STATUS Excute(CGameObject* _Obj, _float _fTimeDelta) = 0;

	void	Set_DecoratorNodes(CNode* _TrueNode, CNode* _FalseNode)
	{
		TrueNode = _TrueNode;
		FalseNode = _FalseNode;
	}
protected:
	CNode* TrueNode = nullptr;
	CNode* FalseNode = nullptr;
public:
	virtual void Free() override
	{
		__super::Free();
		Safe_Release(TrueNode);
		Safe_Release(FalseNode);
	}
};

END


