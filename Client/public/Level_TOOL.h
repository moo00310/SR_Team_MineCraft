#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_TOOL final : public CLevel
{
private:
	CLevel_TOOL(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_TOOL() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Steve(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Grass(const _wstring& strLayerTag);
	HRESULT Ready_Layer_RedTulip(const _wstring& strLayerTag);

public:
	static CLevel_TOOL* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END