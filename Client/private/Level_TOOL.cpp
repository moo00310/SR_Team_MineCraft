#include "Level_TOOL.h"
#include "GameInstance.h"

CLevel_TOOL::CLevel_TOOL(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_TOOL::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Tool(TEXT("Layer_Tool"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_TOOL::Update(_float fTimeDelta)
{
	int a = 10;
}

HRESULT CLevel_TOOL::Render()
{
	SetWindowText(g_hWnd, TEXT("IMGUI 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_TOOL::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_TOOL, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_TOOL, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_TOOL::Ready_Layer_Player(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 50; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_TOOL, TEXT("Prototype_GameObject_Player"),
			LEVEL_TOOL, strLayerTag)))
			return E_FAIL;
	}
	

	return S_OK;
}
HRESULT CLevel_TOOL::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_TOOL, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_TOOL, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_TOOL::Ready_Layer_Tool(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Tool"),
		LEVEL_GAMEPLAY, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CLevel_TOOL* CLevel_TOOL::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_TOOL* pInstance = new CLevel_TOOL(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_TOOL");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_TOOL::Free()
{
	__super::Free();

}
