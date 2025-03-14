#include "Level_YU.h"
#include "GameInstance.h"
#include "MapTool.h"

CLevel_YU::CLevel_YU(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_YU::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Tool(TEXT("Layer_Tool"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_YU::Update(_float fTimeDelta)
{
	bool bCurrentKeyState = (GetKeyState(VK_F1) & 0x8000) != 0;
	if (bCurrentKeyState && !bPrevKeyState)
	{
		dynamic_cast<CMapTool*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Tool"), 0))->SetFrameOnOff();
	}
	bPrevKeyState = bCurrentKeyState;
}

HRESULT CLevel_YU::Render()
{
	SetWindowText(g_hWnd, TEXT("최유경 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_MCTerrain"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Tool(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_MapTool"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CLevel_YU* CLevel_YU::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_YU* pInstance = new CLevel_YU(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_YU");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_YU::Free()
{
	__super::Free();

}
