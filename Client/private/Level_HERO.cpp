#include "Level_HERO.h"
#include "GameInstance.h"

#include "Camera_Player.h"

CLevel_HERO::CLevel_HERO(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_HERO::Initialize()
{
	//if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
  		return E_FAIL;

	if (FAILED(Ready_Layer_HeroCube(TEXT("Layer_HeroCube"))))
 		return E_FAIL;

	if (FAILED(Ready_Layer_Steve(TEXT("Layer_Steve"))))
		return E_FAIL;

	//스티브 다음으로 준비해야 스티브의 트랜스 폼을 찾을 수 있음
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//BT 연습용 적
	if (FAILED(Ready_Layer_HeroEnemy(TEXT("Layer_HeroEnemy"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Sun(TEXT("Layer_Sun"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;*/

	//임무결 1인칭 확인용
	/*if (FAILED(Ready_Laye_MooArm(TEXT("Layer_Arm"))))
		return E_FAIL;*/

	/* 장현우 인벤토리 확인용 */
	if (FAILED(Ready_Layer_WOOInvenTest(TEXT("Layer_Inventory"))))
		return E_FAIL;
	
	m_pGameInstance->Play_Sound("event:/0004 BGM_DOREMI");

	return S_OK;
}

void CLevel_HERO::Update(_float fTimeDelta)
{
	/*if (m_pGameInstance->Key_Down(VK_F5))
	{
		m_pCameraManager->Change_Camera();
	}*/
}

HRESULT CLevel_HERO::Render()
{
	SetWindowText(g_hWnd, TEXT("한영웅 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_HERO::Ready_Layer_Camera(const _wstring& strLayerTag)
{

	CCamera_Player::CAMERA_PLAYER_DESC Cam_Player_Desc{};
	Cam_Player_Desc.vEye = _float3(0.f, 10.f, -10.f);
	Cam_Player_Desc.vAt = _float3(0.f, 0.f, 0.f);
	Cam_Player_Desc.fFov = D3DXToRadian(60.f);
	Cam_Player_Desc.fNear = 0.1f;
	Cam_Player_Desc.fFar = 300.f;
	Cam_Player_Desc.fMouseSensor = 0.1f;
	Cam_Player_Desc.pTarget = m_pGameInstance->Get_Object(LEVEL_HERO, TEXT("Layer_Steve"), 0);//게임인스턴스-> Find Layer-> Steve Layer에서 GameObject* 가져와야 할 듯
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_Camera_Player"),
		LEVEL_HERO, strLayerTag, &Cam_Player_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_HERO::Ready_Layer_Player(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 50; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_Player"),
			LEVEL_HERO, strLayerTag)))
			return E_FAIL;
	}

	return S_OK;
}
HRESULT CLevel_HERO::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
 	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_HERO, strLayerTag)))
  		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_HERO::Ready_Layer_HeroCube(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_HeroCube"),
		LEVEL_HERO, strLayerTag)))
		return E_FAIL;

	for (_uint i = 0; i < 30; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_HeroCubeNoMove"),
			LEVEL_HERO, strLayerTag)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_HERO::Ready_Layer_Steve(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Steve"),
		LEVEL_HERO, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_HERO::Ready_Layer_HeroEnemy(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_HeroEnemy"),
		LEVEL_HERO, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_HERO::Ready_Laye_MooArm(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Arm_Steve"),
		LEVEL_STATIC, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_HERO::Ready_Layer_SkyBox(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_SkyBox"),
		LEVEL_HERO, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_HERO::Ready_Layer_Sun(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_Sun"),
		LEVEL_HERO, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_HERO::Ready_Layer_WOOInvenTest(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_Inventory"),
		LEVEL_HERO, strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_CheckBox"),
		LEVEL_HERO, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CLevel_HERO* CLevel_HERO::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_HERO* pInstance = new CLevel_HERO(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_HERO");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_HERO::Free()
{
	__super::Free();

	//Safe_Release(m_pCameraManager);
}
