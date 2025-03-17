#include "Camera_TPS.h"
#include <DirectXMath.h>
using namespace DirectX;

CCamera_TPS::CCamera_TPS(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera{ pGraphic_Device }
{
}

CCamera_TPS::CCamera_TPS(const CCamera_TPS& Prototype)
	:CCamera(Prototype)
{
}

HRESULT CCamera_TPS::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_TPS::Initialize(void* pArg)
{
	ShowCursor(false);

	/* TransformCom을 생성해놓는다. */
	if (FAILED(Ready_Components()))
		return E_FAIL;

	//아규먼트 받기
	CAMERA_TPS_DESC	Desc{ *static_cast<CAMERA_TPS_DESC*>(pArg) };
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

void CCamera_TPS::Priority_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

}


void CCamera_TPS::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;
}

void CCamera_TPS::Late_Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;

	// 현재 창이 활성화 상태인지 확인
	bool isActive = (GetForegroundWindow() == g_hWnd);

	// 1. 화면 중앙 좌표 계산
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

	// 2. 현재 마우스 좌표 가져오기
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse); // 클라이언트 좌표로 변환

	// 3. 창이 활성화 상태가 아닐 경우 마우스 입력을 무시
	if (!isActive)
		return;

	// === 마우스가 창 내부에 있는지 확인 ===
	if (ptMouse.x < 0 || ptMouse.x >= rc.right || ptMouse.y < 0 || ptMouse.y >= rc.bottom)
		return;  // 창 밖이면 회전 로직을 실행하지 않음

	// 4. 마우스 이동량 계산 (중앙 기준)
	_int iMouseMoveX = ptMouse.x - ptCenter.x;
	_int iMouseMoveY = ptMouse.y - ptCenter.y;

	// 5. 마우스 이동량을 기반으로 카메라 회전 적용
	m_fYaw += iMouseMoveX * fTimeDelta * m_fMouseSensor;
	m_fPitch += iMouseMoveY * fTimeDelta * m_fMouseSensor;
	m_fPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, m_fPitch));

	// 6. 마우스를 다시 중앙으로 이동
	ClientToScreen(g_hWnd, &ptCenter);
	SetCursorPos(ptCenter.x, ptCenter.y);

	// 7. 마우스를 창 내부에 가두기 (ClipCursor 사용)
	RECT clipRect;
	GetWindowRect(g_hWnd, &clipRect);
	ClipCursor(&clipRect); // 마우스를 창 내부에 고정

	// 8. 카메라 위치 업데이트
	_float3 vTargetPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION);
	_float fRadius = 5.0f;
	_float fHeight = 2.0f;

	_float fX = fRadius * cosf(m_fPitch) * sinf(m_fYaw);
	_float fY = fRadius * sinf(m_fPitch) + fHeight;
	_float fZ = fRadius * cosf(m_fPitch) * cosf(m_fYaw);

	_float3 vCameraPos = vTargetPos + _float3(fX, fY, fZ);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);

	_float3 vHeadPos = vTargetPos + _float3(0.f, 1.f, 0.f);
	m_pTransformCom->LookAt(vHeadPos);

	__super::Update_VP_Matrices();
}

HRESULT CCamera_TPS::Render()
{
	if (!m_isActive)
		return S_OK;

	return S_OK;
}

HRESULT CCamera_TPS::Ready_Components()
{
	CTransform::TRANSFORM_DESC		TransformDesc{};

	TransformDesc.fSpeedPerSec = 30.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(180.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CCamera_TPS* CCamera_TPS::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_TPS* pInstance = new CCamera_TPS(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_TPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_TPS::Clone(void* pArg)
{
	CCamera_TPS* pInstance = new CCamera_TPS(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCamera_TPS");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_TPS::Free()
{
	__super::Free();
}
