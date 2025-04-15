#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	_bool isFinished() const {
		return m_isFinished;
	}

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	HRESULT Loading();

	void Output_LoadingText() {
		SetWindowText(g_hWnd, m_szLoadingText);
	}
	
public:
	_int Get_Percent() const {

		if (m_totalCount == 0)
			return 0;

		return (_int)(((_float)m_loadeCount / m_totalCount) * 100.f);
	}

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	LEVEL						m_eNextLevelID = { LEVEL_END };
	_bool						m_isFinished = { false };
	CGameInstance*				m_pGameInstance = { nullptr };

private:
	HANDLE						m_hThread = {};
	CRITICAL_SECTION			m_CriticalSection = {};
	_tchar						m_szLoadingText[MAX_PATH] = {};


private:
	_int m_loadeCount = { 0 };
	_int m_totalCount = 135;



public:
	HRESULT Loading_For_Logo();
	HRESULT Loading_For_GamePlay();
	HRESULT Loading_For_MOOPlay();
	HRESULT Loading_For_YUPlay();
	HRESULT Loading_For_HEROPlay();
	HRESULT Loading_For_WOOPlay();
	HRESULT Loading_For_HECKPlay();
	HRESULT Loading_For_TOOL();
	HRESULT Loading_For_Ending();


public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;
};

END