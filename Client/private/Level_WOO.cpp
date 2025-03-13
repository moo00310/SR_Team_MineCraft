#include "Level_WOO.h"
#include "GameInstance.h"

CLevel_WOO::CLevel_WOO(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_WOO::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_WOO::Update(_float fTimeDelta)
{
	int a = 10;
}

HRESULT CLevel_WOO::Render()
{
	SetWindowText(g_hWnd, TEXT("장현우 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_WOO::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_WOO, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_WOO, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_WOO::Ready_Layer_Player(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 50; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_WOO, TEXT("Prototype_GameObject_Player"),
			LEVEL_WOO, strLayerTag)))
			return E_FAIL;
	}
	

	return S_OK;
}
HRESULT CLevel_WOO::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_WOO, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_WOO, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CLevel_WOO* CLevel_WOO::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_WOO* pInstance = new CLevel_WOO(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_WOO");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_WOO::Free()
{
	__super::Free();

}
