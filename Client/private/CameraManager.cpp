#include "CameraManager.h"
#include "GameInstance.h"
#include "Client_Defines.h"


CCameraManager::CCameraManager()
{
}

HRESULT CCameraManager::Initialize()
{
	_int iIndex{ 0 };
	CCamera* pCamera{ nullptr };

	//모든 카메라를 가져온다.
	while (true)
	{
		pCamera = dynamic_cast<CCamera*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_HERO, TEXT("Layer_Camera"), iIndex));
		
		if (!pCamera)
			break;

		Safe_AddRef(pCamera);
		pCamera->Set_Active(false);
		m_Cameras.push_back(pCamera);

		++iIndex;
	}

	//처음 카메라 부터 시작
	m_CurrentCamera = m_Cameras.begin();
	(*m_CurrentCamera)->Set_Active(true);

	return S_OK;
}

void CCameraManager::Change_Camera()
{
	// 현재 카메라가 바라보고 있는 방향 벡터 저장
	_float3 vCurrentLook = (*m_CurrentCamera)->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	_float3 vCurrentPos = (*m_CurrentCamera)->Get_Transform()->Get_State(CTransform::STATE_POSITION);

	// 현재 Yaw, Pitch 값 저장 (TPS ↔ FPS 전환 시 유지)
	_float fCurrentYaw = (*m_CurrentCamera)->Get_Yaw();
	_float fCurrentPitch = (*m_CurrentCamera)->Get_Pitch();

	// 현재 카메라를 끄고
	(*m_CurrentCamera)->Set_Active(false);

	// 마지막 카메라였다면 처음으로 순환
	auto pNextCamera = ++m_CurrentCamera;
	if (pNextCamera == m_Cameras.end())
	{
		pNextCamera = m_Cameras.begin();
	}

	// 다음 카메라 활성화
	(*pNextCamera)->Set_Active(true);
	m_CurrentCamera = pNextCamera;

	// 새로운 카메라에 기존 카메라 방향을 유지하도록 설정
	(*m_CurrentCamera)->Set_Yaw(fCurrentYaw);
	(*m_CurrentCamera)->Set_Pitch(fCurrentPitch);

	// 새로운 Look 벡터 설정 (Yaw/Pitch 기반으로 적용됨)
	(*m_CurrentCamera)->Get_Transform()->Set_State(CTransform::STATE_LOOK, vCurrentLook);

	// 위치 설정
	(*m_CurrentCamera)->Get_Transform()->Set_State(CTransform::STATE_POSITION, vCurrentPos);
}




CCameraManager* CCameraManager::Create()
{
	CCameraManager* pInstance = new CCameraManager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CCameraManager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCameraManager::Free()
{
	__super::Free();

	for (CCamera* pCamera : m_Cameras)
	{
		Safe_Release(pCamera);
	}

	m_Cameras.clear();
}
