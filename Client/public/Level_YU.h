#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_YU final : public CLevel
{
private:
	CLevel_YU(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_YU() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Terrain(const _wstring& strLayerTag);

public:
	static CLevel_YU* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END