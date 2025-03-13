#include "Level_YU.h"
#include "GameInstance.h"

CLevel_YU::CLevel_YU(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_YU::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_YU::Update(_float fTimeDelta)
{
	int a = 10;
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

HRESULT CLevel_YU::Ready_Layer_Player(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 50; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Player"),
			LEVEL_YU, strLayerTag)))
			return E_FAIL;
	}
	

	return S_OK;
}
HRESULT CLevel_YU::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Terrain"),
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
