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

	//��� ī�޶� �����´�.
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

	//ó�� ī�޶� ���� ����
	m_CurrentCamera = m_Cameras.begin();
	(*m_CurrentCamera)->Set_Active(true);

	return S_OK;
}

void CCameraManager::Change_Camera()
{
	// ���� ī�޶� �ٶ󺸰� �ִ� ���� ���� ����
	_float3 vCurrentLook = (*m_CurrentCamera)->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	_float3 vCurrentPos = (*m_CurrentCamera)->Get_Transform()->Get_State(CTransform::STATE_POSITION);

	// ���� Yaw, Pitch �� ���� (TPS �� FPS ��ȯ �� ����)
	_float fCurrentYaw = (*m_CurrentCamera)->Get_Yaw();
	_float fCurrentPitch = (*m_CurrentCamera)->Get_Pitch();

	// ���� ī�޶� ����
	(*m_CurrentCamera)->Set_Active(false);

	// ������ ī�޶󿴴ٸ� ó������ ��ȯ
	auto pNextCamera = ++m_CurrentCamera;
	if (pNextCamera == m_Cameras.end())
	{
		pNextCamera = m_Cameras.begin();
	}

	// ���� ī�޶� Ȱ��ȭ
	(*pNextCamera)->Set_Active(true);
	m_CurrentCamera = pNextCamera;

	// ���ο� ī�޶� ���� ī�޶� ������ �����ϵ��� ����
	(*m_CurrentCamera)->Set_Yaw(fCurrentYaw);
	(*m_CurrentCamera)->Set_Pitch(fCurrentPitch);

	// ���ο� Look ���� ���� (Yaw/Pitch ������� �����)
	(*m_CurrentCamera)->Get_Transform()->Set_State(CTransform::STATE_LOOK, vCurrentLook);

	// ��ġ ����
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
