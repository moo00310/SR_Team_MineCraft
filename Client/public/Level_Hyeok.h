#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "ParticleEventManager.h"

BEGIN(Client)

class CLevel_Hyeok final : public CLevel
{
private:
	CLevel_Hyeok(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Hyeok() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Hyock(const _wstring& strLayerTag);
	HRESULT Ready_Layer_TestParticle(const _wstring& strLayerTag);
	HRESULT Ready_Layer_DigParticle(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Particle(const _wstring& strLayerTag);

public:
	static CLevel_Hyeok* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END