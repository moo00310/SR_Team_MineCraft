#pragma once

#include "Client_Defines.h"
#include "Level.h"


BEGIN(Client)

class CLevel_HERO final : public CLevel
{
private:
	CLevel_HERO(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_HERO() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_HeroCube(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Steve(const _wstring& strLayerTag);
	HRESULT Ready_Layer_HeroEnemy(const _wstring& strLayerTag);
	HRESULT Ready_Laye_MooArm(const _wstring& strLayerTag);
	HRESULT Ready_Layer_SkyBox(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Sun(const _wstring& strLayerTag);
	HRESULT Ready_Layer_WOOInvenTest(const _wstring& strLayerTag);


private:
	//CCameraManager* m_pCameraManager{ nullptr };

public:
	static CLevel_HERO* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END