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
	HRESULT Ready_Layer_Tool(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Steve(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Sun(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Clouds(const _wstring& strLayerTag);
	HRESULT Ready_Layer_SkyBox(const _wstring& strLayerTag);

	HRESULT Ready_Layer_Particle(const _wstring& strLayerTag);
	HRESULT Ready_Layer_TPS_Arm(const _wstring& strLayerTag);
	HRESULT Ready_Laye_Rect_Model(const _wstring& strLayerTag);
	HRESULT Ready_Laye_Cube_Model(const _wstring& strLayerTag);
	HRESULT Ready_Laye_RightHand(const _wstring& strLayerTag); 	
	HRESULT Ready_Laye_Left_Rect_Model(const _wstring& strLayerTag);

	HRESULT Ready_Layer_Inventory(const _wstring& strLayerTag);
	HRESULT Ready_Layer_PlayerState(const _wstring& strLayerTag);

	HRESULT Ready_Laye_Creeper(const _wstring& strLayerTag);
	HRESULT Ready_Laye_Zombi(const _wstring& strLayerTag);

	HRESULT Ready_Layer_DestroyCube(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI_DropItem(const _wstring& strLayerTag);

private:
	wchar_t m_szFPS[20];
	float  ftime = { 0 };
	int m_iFPS = {};

public:
	static CLevel_YU* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END