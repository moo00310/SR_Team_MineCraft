#include "Level_TOOL.h"
#include "GameInstance.h"
#include "Camera_Player.h"

CLevel_TOOL::CLevel_TOOL(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_TOOL::Initialize()
{
	if (FAILED(Ready_Layer_Steve(TEXT("Layer_Steve"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
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
	CCamera_Player::CAMERA_PLAYER_DESC Cam_Player_Desc{};
	Cam_Player_Desc.vEye = _float3(0.f, 10.f, -10.f);
	Cam_Player_Desc.vAt = _float3(0.f, 0.f, 0.f);
	Cam_Player_Desc.fFov = D3DXToRadian(60.f);
	Cam_Player_Desc.fNear = 0.1f;
	Cam_Player_Desc.fFar = 300.f;
	Cam_Player_Desc.fMouseSensor = 0.1f;
	Cam_Player_Desc.pTarget = m_pGameInstance->Get_Object(LEVEL_TOOL, TEXT("Layer_Steve"), 0);//게임인스턴스-> Find Layer-> Steve Layer에서 GameObject* 가져와야 할 듯
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_TOOL, TEXT("Prototype_GameObject_Camera_Player"),
		LEVEL_TOOL, strLayerTag, &Cam_Player_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_TOOL::Ready_Layer_Steve(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Steve"),
		LEVEL_TOOL, strLayerTag)))
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
