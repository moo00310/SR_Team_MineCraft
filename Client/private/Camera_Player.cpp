#include "Camera_Player.h"
#include <DirectXMath.h>
using namespace DirectX;

CCamera_Player::CCamera_Player(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CCamera{ pGraphic_Device }
{
}

CCamera_Player::CCamera_Player(const CCamera_Player& Prototype)
	:CCamera(Prototype)
{
}

HRESULT CCamera_Player::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Player::Initialize(void* pArg)
{
	ShowCursor(false);

	/* TransformCom 생성 */
	if (FAILED(Ready_Components()))
		return E_FAIL;

	// 아규먼트 받기
	CAMERA_PLAYER_DESC Desc{ *static_cast<CAMERA_PLAYER_DESC*>(pArg) };
	m_fMouseSensor = Desc.fMouseSensor;
	if (!Desc.pTarget)
		return E_FAIL;

	// 플레이어 트랜스폼 받기
	m_pTargetTransformCom = static_cast<CTransform*>(Desc.pTarget->Find_Component(TEXT("Com_Transform")));

	// 카메라 기본 값 세팅
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	// 마우스 올드값 세팅
	GetCursorPos(&m_ptOldMousePos);
	ScreenToClient(g_hWnd, &m_ptOldMousePos);

	// 기본 모드를 TPS로 설정
	m_eCameraMode = E_CAMERA_MODE::TPS;

	return S_OK;
}

void CCamera_Player::Priority_Update(_float fTimeDelta)
{

}

void CCamera_Player::Update(_float fTimeDelta)
{

	// 모드 전환
	if (m_pGameInstance->Key_Down(VK_F5))
	{
		m_eCameraMode = (m_eCameraMode == E_CAMERA_MODE::FPS) ? E_CAMERA_MODE::TPS : E_CAMERA_MODE::FPS;
	}
}

void CCamera_Player::Late_Update(_float fTimeDelta)
{
	// 창이 활성화 상태가 아닐 경우 마우스 입력을 무시
	if (!(GetForegroundWindow() == g_hWnd))
		return;

	// 화면 중앙 좌표 계산
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

	// 현재 마우스 좌표 가져오기
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	// === 마우스가 창 내부에 있는지 확인 ===
	if (ptMouse.x < 0 || ptMouse.x >= rc.right || ptMouse.y < 0 || ptMouse.y >= rc.bottom)
		return;

	// 마우스 이동량 계산 (중앙 기준)
	_int iMouseMoveX = ptMouse.x - ptCenter.x;
	_int iMouseMoveY = ptMouse.y - ptCenter.y;

	// 마우스 이동량을 기반으로 카메라 회전 적용
	m_fYaw += iMouseMoveX * fTimeDelta * m_fMouseSensor;
	m_fPitch -= iMouseMoveY * fTimeDelta * m_fMouseSensor;

	// 피치(Pitch) 값 제한 (위아래 회전 각도 제한)
	m_fPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, m_fPitch));

	// 마우스를 다시 중앙으로 이동
	ClientToScreen(g_hWnd, &ptCenter);
	SetCursorPos(ptCenter.x, ptCenter.y);

	// 마우스를 창 내부에 가두기 (ClipCursor 사용)
	RECT clipRect;
	GetWindowRect(g_hWnd, &clipRect);
	ClipCursor(&clipRect);

	// 카메라 모드에 따른 위치 및 방향 설정
	_float3 vPlayerPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vLook;
	vLook.x = cosf(m_fPitch) * sinf(m_fYaw);
	vLook.y = sinf(m_fPitch);
	vLook.z = cosf(m_fPitch) * cosf(m_fYaw);

	if (m_eCameraMode == E_CAMERA_MODE::FPS)
	{
		// FPS 모드 (1인칭)
		_float3 vCameraPos = vPlayerPos + _float3(0.f, 1.8f, 0.f);  // 플레이어 머리 위치
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);
		m_pTransformCom->LookAt(vCameraPos + vLook);
	}
	else
	{
		// TPS 모드 (3인칭)
		_float3 vCameraOffset = -vLook * 5.0f;  // 캐릭터에서 일정 거리 뒤쪽
		_float3 vCameraPos = vPlayerPos + vCameraOffset + _float3(0.f, 1.8f, 0.f); // 살짝 위쪽
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);
		m_pTransformCom->LookAt(vPlayerPos + _float3(0.f, 1.5f, 0.f)); // 캐릭터 머리 중심 바라봄
	}

	__super::Update_VP_Matrices();
}

HRESULT CCamera_Player::Render()
{
	return S_OK;
}

HRESULT CCamera_Player::Ready_Components()
{
	CTransform::TRANSFORM_DESC TransformDesc{};
	TransformDesc.fSpeedPerSec = 30.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(180.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CCamera_Player* CCamera_Player::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Player* pInstance = new CCamera_Player(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Player::Clone(void* pArg)
{
	CCamera_Player* pInstance = new CCamera_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCamera_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Player::Free()
{
	__super::Free();
}
