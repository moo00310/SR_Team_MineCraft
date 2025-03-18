#include "CameraManager.h"
#include "GameInstance.h"
#include "Client_Defines.h"


CCameraManager::CCameraManager()
{
}

HRESULT CCameraManager::Initialize(_uint iLevelIndex)
{
	_int iIndex{ 0 };
	CCamera* pCamera{ nullptr };

	//모든 카메라를 가져온다.
	while (true)
	{
		pCamera = dynamic_cast<CCamera*>(CGameInstance::Get_Instance()->Get_Object(iLevelIndex, TEXT("Layer_Camera"), iIndex));
		
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
	//현재 카메라를 끄고
	(*m_CurrentCamera)->Set_Active(false);

	//마지막 카메라 였으면 처음부터
	auto pNextCamera = ++m_CurrentCamera;
	if (pNextCamera == m_Cameras.end())
	{
		pNextCamera = m_Cameras.begin();
	}
	//다음 카메라 활성화
	(*pNextCamera)->Set_Active(true);

	m_CurrentCamera = pNextCamera;
}

CCameraManager* CCameraManager::Create(_uint iLevelIndex)
{
	CCameraManager* pInstance = new CCameraManager();

	if (FAILED(pInstance->Initialize(iLevelIndex)))
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
