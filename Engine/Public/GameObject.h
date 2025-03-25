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
	/* 원형이 호출하는 함수. 오래걸릴 수 있는 초기화작업 */
	virtual HRESULT Initialize_Prototype();
	/* 사본이 호출하는 함수. */
	virtual HRESULT Initialize(void* pArg);

	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

	// 풀링에서 Push시 호출.
	virtual void OnPushPool(){}

public:
	class CComponent* Find_Component(const _tchar* pComponentTag);
	class CGameInstance* Get_GameInstance();

public:
	_bool	Get_isDestroy();
	void	Destroy();

	/// <summary>
	/// Update쪽에 써야하는 경우 Priority_Update쪽에 쓰는걸 권장함.
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

	// 오브젝트 true = 활성화/ false = 비활성화.
	_bool	m_isActive = { true };

protected:
	HRESULT Add_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END