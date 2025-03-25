#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	/* ������ ȣ���ϴ� �Լ�. �����ɸ� �� �ִ� �ʱ�ȭ�۾� */
	virtual HRESULT Initialize_Prototype();
	/* �纻�� ȣ���ϴ� �Լ�. */
	virtual HRESULT Initialize(void* pArg);

	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	// Ǯ������ Push�� ȣ��.
	virtual void OnPushPool(){}

public:
	class CComponent* Find_Component(const _tchar* pComponentTag);
	class CGameInstance* Get_GameInstance();

public:
	_bool	Get_isDestroy();
	void	Destroy();

	/// <summary>
	/// Update�ʿ� ����ϴ� ��� Priority_Update�ʿ� ���°� ������.
	/// </summary>	
	void	SetActive(_bool isActive);
	_bool	GetActive() const;

protected:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };
	class CGameInstance*			m_pGameInstance = { nullptr };

protected:
	map<const _wstring, class CComponent*>	m_Components;

private:
	_bool	m_isDestroyed{ false };

	// ������Ʈ true = Ȱ��ȭ/ false = ��Ȱ��ȭ.
	_bool	m_isActive = { true };

protected:
	HRESULT Add_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END