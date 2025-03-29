#include "Camera_Player.h"
#include <DirectXMath.h>
using namespace DirectX;

#include "BreakableCube.h"
#include "BreakableRect.h"

#include "Monster.h"

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
	m_pTarget_Transform_Com = static_cast<CTransform*>(Desc.pTarget->Find_Component(TEXT("Com_Transform")));
    Safe_AddRef(m_pTarget_Transform_Com);
    
    // 플레이어 리지드바디 받기
    m_pTarget_Rigidbody_Com = static_cast<CRigidbody*>(Desc.pTarget->Find_Component(TEXT("Com_Rigidbody")));
    Safe_AddRef(m_pTarget_Rigidbody_Com);

	// 카메라 기본 값 세팅
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	// 마우스 올드값 세팅
	GetCursorPos(&m_ptOldMousePos);
	ScreenToClient(g_hWnd, &m_ptOldMousePos);

	// 기본 모드를 TPS로 설정
	m_eCameraMode = E_CAMERA_MODE::TPS;

    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION));

    //m_vCurrentCameraPos = m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION);

	return S_OK;
}

void CCamera_Player::Priority_Update(_float fTimeDelta)
{

}

void CCamera_Player::Update(_float fTimeDelta)
{
    Input_Key(fTimeDelta);
    Follow_Player(fTimeDelta);
    __super::Update_VP_Matrices();
}

void CCamera_Player::Late_Update(_float fTimeDelta)
{
    
    if (m_pGameInstance->Key_Down(VK_F5))
    {
        m_eCameraMode = (m_eCameraMode == E_CAMERA_MODE::FPS) ? E_CAMERA_MODE::TPS : E_CAMERA_MODE::FPS;
    }
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
        _float fDist;                  // 광선과 오브젝트 간의 거리
        CGameObject* pHitObject;       // 충돌한 오브젝트
        CComponent* pHitComponent;     // 충돌한 컴포넌트 (콜라이더)

        // Ray Casting: Instancing된 오브젝트와 충돌 검사
        pHitObject = m_pGameInstance->Ray_Cast_InstancingObject(
            m_vHeadPos, // 시작 위치 (카메라 또는 플레이어의 머리 위치)
            m_pTransformCom->Get_State(CTransform::STATE_LOOK), // 시선 방향
            5.f, // 최대 탐색 거리
            COLLISION_BLOCK, // 충돌 그룹
            fDist, // 충돌한 거리 저장
            &pHitComponent // 충돌한 콜라이더 저장
        );

        if (pHitObject)
        {
            if (CBreakableRect* pBreakableRect = dynamic_cast<CBreakableRect*>(pHitObject)) {
                pBreakableRect->Destroy();
                return;
            }

            // 충돌한 오브젝트가 CBreakableCube인지 확인 후 형변환
            if (CBreakableCube* pBreakableCube = dynamic_cast<CBreakableCube*>(pHitObject)) {
                // 충돌한 콜라이더를 CCollider_Cube로 형변환
                CCollider_Cube* pCollider_Cube = static_cast<CCollider_Cube*>(pHitComponent);
                if (!pCollider_Cube)
                    return;

                // 충돌한 콜라이더의 위치를 가져와 해당 블록 삭제
                _float3 hitPosition{ pCollider_Cube->Get_Offset()};

                if (FAILED(pBreakableCube->Delete_Cube(hitPosition)))
                {
                    MSG_BOX("Delete_Cube: Fail");
                }

                return;
            }
        }

        // 몬스터와 충돌 검사
        pHitObject = m_pGameInstance->Ray_Cast(
            m_vHeadPos, // 시작 위치 (카메라 또는 플레이어의 머리 위치)
            m_pTransformCom->Get_State(CTransform::STATE_LOOK), // 시선 방향
            5.f, // 최대 탐색 거리
            COLLISION_MONSTER, // 충돌 그룹
            fDist); //거리저장

        if (pHitObject)
        {
            if (CMonster* monster = dynamic_cast<CMonster*>(pHitObject))
            {
                monster->Nuck_Back();
            }

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

void CCamera_Player::Follow_Player(_float fTimeDelta)
{
    if (!m_pTarget_Transform_Com)
        return;

    // === 플레이어의 회전 각도 가져오기 ===
    _float3 vLook = m_pTarget_Transform_Com->Get_State(CTransform::STATE_LOOK);
    m_fYaw = atan2f(vLook.x, vLook.z);

    // === 카메라 회전 벡터 생성 ===
    _float3 vLookDir = {
        cosf(m_fPitch) * sinf(m_fYaw),
        sinf(m_fPitch),
        cosf(m_fPitch) * cosf(m_fYaw)
    };

    // === 걷는 애니메이션 타이머 ===
    _float3 vVelocity = m_pTarget_Rigidbody_Com->Get_Velocity();
    _float fSpeed = sqrtf(vVelocity.x * vVelocity.x + vVelocity.z * vVelocity.z);
    m_fWalkTime += 2.f * fSpeed * fTimeDelta;
    if (m_fWalkTime > D3DX_PI * 2.f)
        m_fWalkTime -= D3DX_PI * 2.f;

    // === 좌우 & 위아래 흔들림 계산 ===
    _float fShakeOffset_X = cosf(m_fWalkTime) * 0.05f;
    _float fShakeOffset_Y = fabs(sinf(m_fWalkTime) * 0.05f);

    // === 플레이어의 머리 위치 설정 ===
    _float3 vRight = { cosf(m_fYaw), 0.f, -sinf(m_fYaw) };
    _float3 playerPos = m_pTarget_Transform_Com->Get_State(CTransform::STATE_POSITION);
    _float headHeight = 1.8f;
    m_vHeadPos = playerPos + _float3(0.f, headHeight, 0.f) + vRight * fShakeOffset_X + _float3(0.f, fShakeOffset_Y, 0.f);

    if (m_eCameraMode == E_CAMERA_MODE::FPS)
    {
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vHeadPos);
        m_pTransformCom->LookAt(m_vHeadPos + vLookDir);
    }
    else if (m_eCameraMode == E_CAMERA_MODE::TPS)
    {
        // === 3인칭 스프링 암 거리 조절 ===
        _float fTargetDist = m_fSpringArmLength;
        CGameObject* pGameObject = m_pGameInstance->Ray_Cast(m_vHeadPos, -vLookDir, m_fSpringArmLength, COLLISION_BLOCK, fTargetDist);

        // Ray_Cast 결과 확인
        if (pGameObject)
        {
            fTargetDist = clamp(fTargetDist, 0.5f, m_fSpringArmLength);
        }
        else
        {
            // 충돌이 없을 경우, 기본 거리 사용
            fTargetDist = m_fSpringArmLength;
        }

        // === 최종 카메라 위치 계산 ===
        _float3 vFinalCameraPos = m_vHeadPos + (-vLookDir * fTargetDist);

        // === 카메라 위치 바로 적용 ===
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFinalCameraPos);
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

    // 마우스 이동량 계산 (중앙 기준)
    _int iMouseMoveY = ptMouse.y - ptCenter.y;
    _int iMouseMoveX = ptMouse.x - ptCenter.x;

    m_pTarget_Transform_Com->Turn({ 0.f, 1.f, 0.f }, fTimeDelta * iMouseMoveX * 0.05f);

    // Pitch 값 업데이트 (상하 회전)
    m_fPitch -= iMouseMoveY * fTimeDelta * m_fMouseSensor;
    m_fPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, m_fPitch)); // 상하 회전 제한

    // 마우스를 다시 중앙으로 이동
    ClientToScreen(g_hWnd, &ptCenter);
    SetCursorPos(ptCenter.x, ptCenter.y);

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

    Safe_Release(m_pTarget_Transform_Com);
    Safe_Release(m_pTarget_Rigidbody_Com);
}
