#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)


class CLevel_Moo final : public CLevel
{
private:
	CLevel_Moo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Moo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Creeper(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Steve(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Tree(const _wstring& strLayerTag);

public:
	static CLevel_Moo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END