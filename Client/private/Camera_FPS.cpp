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
    _float fYawDelta = iMouseMoveX * fTimeDelta * m_fMouseSensor;
    _float fPitchDelta = iMouseMoveY * fTimeDelta * m_fMouseSensor;

    // 현재 카메라의 look 방향을 _float3로 가져옴
    _float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

    // 피치(Pitch) 값 제한 (위아래 회전 각도 제한)
    fPitchDelta = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, fPitchDelta));

    // 회전 계산: yaw와 pitch에 맞춰 look 벡터 계산
    _float fYaw = atan2f(vLook.z, vLook.x) + fYawDelta;  // 현재 yaw에 delta 추가
    _float fPitch = asinf(vLook.y) + fPitchDelta;  // 현재 pitch에 delta 추가

    // 새 look 벡터 계산
    vLook.x = cosf(fPitch) * sinf(fYaw);
    vLook.y = sinf(fPitch);
    vLook.z = cosf(fPitch) * cosf(fYaw);

    // Look 벡터 정규화
    XMVECTOR vLookVector = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&vLook));
    vLookVector = XMVector3Normalize(vLookVector);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&vLook), vLookVector);

    // 카메라 회전 적용 (Look 벡터 설정)
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

    // 6. 마우스를 다시 중앙으로 이동
    ClientToScreen(g_hWnd, &ptCenter);
    SetCursorPos(ptCenter.x, ptCenter.y);

    // 7. 마우스를 창 내부에 가두기 (ClipCursor 사용)
    RECT clipRect;
    GetWindowRect(g_hWnd, &clipRect);
    ClipCursor(&clipRect); // 마우스를 창 내부에 고정

    // 8. FPS 카메라의 위치 설정 (플레이어의 머리 위치)
    _float3 vPlayerPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION);
    _float3 vCameraPos = vPlayerPos + _float3(0.f, 1.8f, 0.f);  // 플레이어 머리 위치

    // 9. 카메라 위치 적용
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);  // 카메라 위치 설정
    m_pTransformCom->LookAt(vCameraPos + vLook);  // 1인칭 카메라는 자신의 앞을 바라봄

    __super::Update_VP_Matrices();
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
