#include "Camera_FPS.h"
#include <DirectXMath.h>
using namespace DirectX;

CCamera_FPS::CCamera_FPS(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera{ pGraphic_Device }
{
}

CCamera_FPS::CCamera_FPS(const CCamera_FPS& Prototype)
	:CCamera(Prototype)
{
}

HRESULT CCamera_FPS::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_FPS::Initialize(void* pArg)
{
	ShowCursor(false);

	/* TransformCom을 생성해놓는다. */
	if (FAILED(Ready_Components()))
		return E_FAIL;

	//아규먼트 받기
	CAMERA_FPS_DESC	Desc{ *static_cast<CAMERA_FPS_DESC*>(pArg) };
	m_fMouseSensor = Desc.fMouseSensor;
	if (!Desc.pTarget)
		return E_FAIL;
	//플레이어 트랜스폼 받기
	m_pTargetTransformCom = static_cast<CTransform*>(Desc.pTarget->Find_Component(TEXT("Com_Transform")));


	//카메라 기본 값 세팅
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	//마우스 올드값 세팅
	GetCursorPos(&m_ptOldMousePos);
	ScreenToClient(g_hWnd, &m_ptOldMousePos);

	return S_OK;
}

void CCamera_FPS::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	// 1. 화면 중앙 좌표 계산
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

	// 2. 현재 마우스 좌표 가져오기
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse); // 클라이언트 좌표로 변환

	// 3. 마우스 이동량 계산 (중앙 기준)
	_int iMouseMoveX = ptMouse.x - ptCenter.x;
	_int iMouseMoveY = ptMouse.y - ptCenter.y;

	// 4. 마우스 이동량을 기반으로 카메라 회전 적용
	m_fYaw += iMouseMoveX * fTimeDelta * m_fMouseSensor;
	m_fPitch -= iMouseMoveY * fTimeDelta * m_fMouseSensor;

	// 피치(Pitch) 값 제한 (위아래 회전 각도 제한)
	m_fPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, m_fPitch));

	// 5. 마우스 커서를 다시 화면 중앙으로 이동 (마우스 고정)
	ClientToScreen(g_hWnd, &ptCenter);
	SetCursorPos(ptCenter.x, ptCenter.y);

	// 6. FPS 카메라의 위치 설정 (플레이어의 머리 위치)
	_float3 vPlayerPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vCameraPos = vPlayerPos + _float3(0.f, 1.8f, 0.f);  // 플레이어 머리 위치

	// 7. 카메라 방향 벡터 계산 (1인칭)
	_float3 vLook;
	vLook.x = cosf(m_fPitch) * sinf(m_fYaw);
	vLook.y = sinf(m_fPitch);
	vLook.z = cosf(m_fPitch) * cosf(m_fYaw);
	XMVECTOR vLookVector = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&vLook));
	vLookVector = XMVector3Normalize(vLookVector);
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&vLook), vLookVector);

	// 8. 카메라 방향 적용
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);
	m_pTransformCom->LookAt(vCameraPos + vLook);  // 1인칭 카메라는 자신의 앞을 바라봄

	__super::Update_VP_Matrices();
}



void CCamera_FPS::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
}

void CCamera_FPS::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
}

HRESULT CCamera_FPS::Render()
{
	if (!m_isActive)
		return S_OK;

	return S_OK;
}

HRESULT CCamera_FPS::Ready_Components()
{
	CTransform::TRANSFORM_DESC		TransformDesc{};

	TransformDesc.fSpeedPerSec = 30.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(180.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CCamera_FPS* CCamera_FPS::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_FPS* pInstance = new CCamera_FPS(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_FPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_FPS::Clone(void* pArg)
{
	CCamera_FPS* pInstance = new CCamera_FPS(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCamera_FPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_FPS::Free()
{
	__super::Free();
}
