#include "Level_YU.h"
#include "GameInstance.h"
#include "MapTool.h"

#include "Camera_Player.h"
#include "Item_Model.h"


#include "Steve.h"

CLevel_YU::CLevel_YU(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_YU::Initialize()
{

	if (FAILED(Ready_Layer_Steve(TEXT("Layer_Steve"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Clouds(TEXT("Layer_Clouds"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	
	if (FAILED(Ready_Laye_Sword(TEXT("Layer_Item"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Sun(TEXT("Layer_Sun"))))
		return E_FAIL;



	return S_OK;
}

void CLevel_YU::Update(_float fTimeDelta)
{

}

HRESULT CLevel_YU::Render()
{
	SetWindowText(g_hWnd, TEXT("최유경 레벨입니다."));

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

HRESULT CLevel_YU::Ready_Layer_Steve(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Steve"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	dynamic_cast<CSteve*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Steve"), 0))->SetPos(_float3(0, 20, 0));

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Sun(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Sun"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Clouds(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Clouds"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

//HRESULT CLevel_YU::Ready_Layer_Camera(const _wstring& strLayerTag) {
//	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Camera_Free"),
//		LEVEL_YU, strLayerTag)))
//		return E_FAIL;
//
//	return S_OK;
//
//}

HRESULT CLevel_YU::Ready_Laye_Sword(const _wstring& strLayerTag)
{
	CItem_Model::DESC desc
	{
		TEXT("Prototype_Component_Texture_Pickaxe"),
		LEVEL_YU
	};

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Sword"),
		LEVEL_YU, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_YU::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Player::CAMERA_PLAYER_DESC Cam_Player_Desc{};
	Cam_Player_Desc.vEye = _float3(0.f, 10.f, -10.f);
	Cam_Player_Desc.vAt = _float3(0.f, 0.f, 0.f);
	Cam_Player_Desc.fFov = D3DXToRadian(60.f);
	Cam_Player_Desc.fNear = 0.1f;
	Cam_Player_Desc.fFar = 300.f;
	Cam_Player_Desc.fMouseSensor = 0.1f;
	Cam_Player_Desc.pTarget = m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Steve"), 0);//게임인스턴스-> Find Layer-> Steve Layer에서 GameObject* 가져와야 할 듯
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Camera_Player"),
		LEVEL_YU, strLayerTag, &Cam_Player_Desc)))
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
