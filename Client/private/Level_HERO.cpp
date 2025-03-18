#include "Level_HERO.h"
#include "GameInstance.h"

#include "Camera_TPS.h"
#include "Camera_FPS.h"

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

	//임무결 1인칭 확인용
	if (FAILED(Ready_Laye_MooArm(TEXT("Layer_Arm"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_HERO::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(VK_F5))
	{
		m_pCameraManager->Change_Camera();
	}
}

HRESULT CLevel_HERO::Render()
{
	SetWindowText(g_hWnd, TEXT("한영웅 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_HERO::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_TPS::CAMERA_TPS_DESC Cam_TPS_Desc{};
	Cam_TPS_Desc.vEye = _float3(0.f, 10.f, -10.f);
	Cam_TPS_Desc.vAt = _float3(0.f, 0.f, 0.f);
	Cam_TPS_Desc.fFov = D3DXToRadian(60.f);
	Cam_TPS_Desc.fNear = 0.1f;
	Cam_TPS_Desc.fFar = 300.f;
	Cam_TPS_Desc.fMouseSensor = 0.1f;
	Cam_TPS_Desc.pTarget = m_pGameInstance->Get_Object(LEVEL_HERO, TEXT("Layer_Steve"), 0);//게임인스턴스-> Find Layer-> Steve Layer에서 GameObject* 가져와야 할 듯
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_Camera_TPS"),
		LEVEL_HERO, strLayerTag,&Cam_TPS_Desc)))
		return E_FAIL;

	CCamera_FPS::CAMERA_FPS_DESC Cam_FPS_Desc{};
	Cam_FPS_Desc.vEye = _float3(0.f, 10.f, -10.f);
	Cam_FPS_Desc.vAt = _float3(0.f, 0.f, 0.f);
	Cam_FPS_Desc.fFov = D3DXToRadian(60.f);
	Cam_FPS_Desc.fNear = 0.1f;
	Cam_FPS_Desc.fFar = 300.f;
	Cam_FPS_Desc.fMouseSensor = 0.1f;
	Cam_FPS_Desc.pTarget = m_pGameInstance->Get_Object(LEVEL_HERO, TEXT("Layer_Steve"), 0);//게임인스턴스-> Find Layer-> Steve Layer에서 GameObject* 가져와야 할 듯
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_Camera_FPS"),
		LEVEL_HERO, strLayerTag, &Cam_FPS_Desc)))
		return E_FAIL;

	//카메라 매니저에다가 카메라 Layer의 모든 카메라를 가져온다음
	//특정 키를 누를 때마다 다음 카메라만 활성화시킨다.
	m_pCameraManager = CCameraManager::Create();

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

	/*if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HERO, TEXT("Prototype_GameObject_HeroCubeNoMove"),
		LEVEL_HERO, strLayerTag)))
		return E_FAIL;*/

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

	Safe_Release(m_pCameraManager);
}
