#include "Level_Ending.h"
#include "GameInstance.h"

#include "Camera_Player.h"

CLevel_Ending::CLevel_Ending(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{

}

HRESULT CLevel_Ending::Initialize()
{

	return S_OK;
}

void CLevel_Ending::Update(_float fTimeDelta)
{

}

HRESULT CLevel_Ending::Render()
{
	SetWindowText(g_hWnd, TEXT("엔딩 레벨입니다."));

	return S_OK;
}

CLevel_Ending* CLevel_Ending::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Ending* pInstance = new CLevel_Ending(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Ending");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Ending::Free()
{
	__super::Free();

}
