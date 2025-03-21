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
    m_isActiveMouse = false;
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
    Input_Key(fTimeDelta);

	// 모드 전환
	if (m_pGameInstance->Key_Down(VK_F5))
	{
		m_eCameraMode = (m_eCameraMode == E_CAMERA_MODE::FPS) ? E_CAMERA_MODE::TPS : E_CAMERA_MODE::FPS;
	}
}

void CCamera_Player::Late_Update(_float fTimeDelta)
{
    Follow_Player();

    __super::Update_VP_Matrices();
}


HRESULT CCamera_Player::Render()
{
	return S_OK;
}

void CCamera_Player::Input_Key(_float fTimeDelta)
{
    // 창이 활성화 상태가 아닐 경우 입력을 무시
    if (!(GetForegroundWindow() == g_hWnd))
        return;

    if (!m_isActiveMouse)
    {
        On_MouseMove(fTimeDelta);
    }

    if (m_pGameInstance->Key_Down(VK_LBUTTON))
    {
        _float fDist;
        CGameObject* pHitObject;

        pHitObject =  m_pGameInstance->Ray_Cast(
            m_vHeadPos,
            m_pTransformCom->Get_State(CTransform::STATE_LOOK),
            5.f,
            COLLISION_BLOCK,
            fDist);

        if (pHitObject)
        {
            if (pHitObject)
                pHitObject->Destroy();
        }

    }

    if (m_pGameInstance->Key_Down(VK_ESCAPE))
    {
        if (m_isActiveMouse)
        {
            m_isActiveMouse = false;
            ShowCursor(false);

            RECT rc;
            GetClientRect(g_hWnd, &rc);
            POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

            // 마우스를 다시 중앙으로 이동
            ClientToScreen(g_hWnd, &ptCenter);
            SetCursorPos(ptCenter.x, ptCenter.y);
        }
        else
        {
            m_isActiveMouse = true;
            ShowCursor(true);
        }
    }
}

void CCamera_Player::Follow_Player()
{
    if (!m_pTargetTransformCom)
        return;

    // === 스티브의 Yaw 값 가져오기 ===
    if (m_pTargetTransformCom) // 스티브의 Transform 컴포넌트가 존재하는지 확인
    {
        _float3 vLook = m_pTargetTransformCom->Get_State(CTransform::STATE_LOOK); // Look 벡터 가져오기

        // Yaw 값 계산 (atan2 사용)
        m_fYaw = atan2f(vLook.x, vLook.z); // X, Z를 이용해 Yaw 값 추출
    }

    // === 카메라 회전 행렬 적용 ===
    _float3 vLook;
    vLook.x = cosf(m_fPitch) * sinf(m_fYaw);
    vLook.y = sinf(m_fPitch);
    vLook.z = cosf(m_fPitch) * cosf(m_fYaw);


    m_vHeadPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(0.f, 1.5f, 0.f);

    if (m_eCameraMode == E_CAMERA_MODE::FPS)
    {
        // 1인칭(FPS) 모드
        //_float3 vCameraPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(0.f, 1.8f, 0.f);
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vHeadPos);
        m_pTransformCom->LookAt(m_vHeadPos + vLook);
    }
    else
    {
        // 3인칭(TPS) 모드
        //_float3 vPlayerPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION);
        _float3 vCameraOffset = -vLook * 5.0f;
        _float3 vCameraPos = m_vHeadPos + vCameraOffset;
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);
        m_pTransformCom->LookAt(m_vHeadPos);
    }

}

void CCamera_Player::On_MouseMove(_float fTimeDelta)
{
    // === 화면 중앙 좌표 계산 ===
    RECT rc;
    GetClientRect(g_hWnd, &rc);
    POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

    // 현재 마우스 좌표 가져오기
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    //// === 마우스가 창 내부에 있는지 확인 ===
    //if (ptMouse.x < 0 || ptMouse.x >= rc.right || ptMouse.y < 0 || ptMouse.y >= rc.bottom)
    //    return;

    // 마우스 이동량 계산 (중앙 기준)
    _int iMouseMoveY = ptMouse.y - ptCenter.y;

    // Pitch 값 업데이트 (상하 회전)
    m_fPitch -= iMouseMoveY * fTimeDelta * m_fMouseSensor;
    m_fPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, m_fPitch)); // 상하 회전 제한

    // 마우스를 다시 중앙으로 이동
    ClientToScreen(g_hWnd, &ptCenter);
    SetCursorPos(ptCenter.x, ptCenter.y);

    //// 마우스를 창 내부에 가두기 (ClipCursor 사용)
    //RECT clipRect;
    //GetWindowRect(g_hWnd, &clipRect);
    //ClipCursor(&clipRect);
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
