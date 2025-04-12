#include "Camera_Cutscene.h"
#include "Right_hand.h"

Camera_Cutscene::Camera_Cutscene(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera{ pGraphic_Device }
{
}

Camera_Cutscene::Camera_Cutscene(const Camera_Cutscene& Prototype)
	: CCamera(Prototype)
{
}

HRESULT Camera_Cutscene::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Camera_Cutscene::Initialize(void* pArg)
{
	/* TransformCom을 생성해놓는다. */
	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAMERA_FREE_DESC	Desc{};

	Desc.vEye = _float3(0.f, 10.f, -10.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);
	Desc.fFov = D3DXToRadian(60.f);
	Desc.fNear = 0.1f;
	Desc.fFar = 300.f;
;
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	SetActive(false);

	return S_OK;
}

void Camera_Cutscene::Priority_Update(_float fTimeDelta)
{

}

void Camera_Cutscene::Update(_float fTimeDelta)
{
	m_pGameInstance->UpdateListener(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_UP));

	// 누적 각도
	m_fAngle += fTimeDelta * m_fSpeed; // 시간에 따라 회전

	// 공전 거리
	const _float fRadius = 5.f;

	// 중심(m_vLook)을 기준으로 공전 궤도 상 위치 계산
	_float3 vPos;
	vPos.x = m_vLook.x + fRadius * cosf(m_fAngle);
	vPos.z = m_vLook.z + fRadius * sinf(m_fAngle);
	vPos.y = m_vLook.y + 2.f; // 필요에 따라 높이 조절

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom->LookAt(m_vLook);

	if (m_fAngle > D3DXToRadian(270.f))
	{
		m_fAngle = 0.f;
		End_Cutscene();
	}

	__super::Update_VP_Matrices();
}


void Camera_Cutscene::Late_Update(_float fTimeDelta)
{

}

HRESULT Camera_Cutscene::Render()
{


	return S_OK;
}

void Camera_Cutscene::Start_Cutscene(_float3 vPos)
{
	m_vLook = vPos;

	m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Camera"))->SetActive(false);
	static_cast<CRight_hand*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_RightHand_GameObject")))->OffRightHands();
	m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_RightHand_GameObject"))->SetActive(false);
	m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Camera_Cutscene"))->SetActive(true);

}

void Camera_Cutscene::End_Cutscene()
{
	m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Camera_Cutscene"))->SetActive(false);
	m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_RightHand_GameObject"))->SetActive(true);
	m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Camera"))->SetActive(true);
}

HRESULT Camera_Cutscene::Ready_Components()
{
	CTransform::TRANSFORM_DESC		TransformDesc{};

	TransformDesc.fSpeedPerSec = 30.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(180.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

Camera_Cutscene* Camera_Cutscene::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	Camera_Cutscene* pInstance = new Camera_Cutscene(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Camera_Cutscene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* Camera_Cutscene::Clone(void* pArg)
{
	Camera_Cutscene* pInstance = new Camera_Cutscene(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : Camera_Cutscene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Camera_Cutscene::Free()
{
	__super::Free();


}
