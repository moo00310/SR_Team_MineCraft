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

	/* TransformCom ���� */
	if (FAILED(Ready_Components()))
		return E_FAIL;

	// �ƱԸ�Ʈ �ޱ�
	CAMERA_PLAYER_DESC Desc{ *static_cast<CAMERA_PLAYER_DESC*>(pArg) };
	m_fMouseSensor = Desc.fMouseSensor;
	if (!Desc.pTarget)
 		return E_FAIL;

	// �÷��̾� Ʈ������ �ޱ�
	m_pTargetTransformCom = static_cast<CTransform*>(Desc.pTarget->Find_Component(TEXT("Com_Transform")));

	// ī�޶� �⺻ �� ����
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	// ���콺 �õ尪 ����
	GetCursorPos(&m_ptOldMousePos);
	ScreenToClient(g_hWnd, &m_ptOldMousePos);

	// �⺻ ��带 TPS�� ����
	m_eCameraMode = E_CAMERA_MODE::TPS;

	return S_OK;
}

void CCamera_Player::Priority_Update(_float fTimeDelta)
{

}

void CCamera_Player::Update(_float fTimeDelta)
{
    Input_Key(fTimeDelta);

	// ��� ��ȯ
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
    // â�� Ȱ��ȭ ���°� �ƴ� ��� �Է��� ����
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

            // ���콺�� �ٽ� �߾����� �̵�
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

    // === ��Ƽ���� Yaw �� �������� ===
    if (m_pTargetTransformCom) // ��Ƽ���� Transform ������Ʈ�� �����ϴ��� Ȯ��
    {
        _float3 vLook = m_pTargetTransformCom->Get_State(CTransform::STATE_LOOK); // Look ���� ��������

        // Yaw �� ��� (atan2 ���)
        m_fYaw = atan2f(vLook.x, vLook.z); // X, Z�� �̿��� Yaw �� ����
    }

    // === ī�޶� ȸ�� ��� ���� ===
    _float3 vLook;
    vLook.x = cosf(m_fPitch) * sinf(m_fYaw);
    vLook.y = sinf(m_fPitch);
    vLook.z = cosf(m_fPitch) * cosf(m_fYaw);


    m_vHeadPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(0.f, 1.5f, 0.f);

    if (m_eCameraMode == E_CAMERA_MODE::FPS)
    {
        // 1��Ī(FPS) ���
        //_float3 vCameraPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION) + _float3(0.f, 1.8f, 0.f);
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vHeadPos);
        m_pTransformCom->LookAt(m_vHeadPos + vLook);
    }
    else
    {
        // 3��Ī(TPS) ���
        //_float3 vPlayerPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION);
        _float3 vCameraOffset = -vLook * 5.0f;
        _float3 vCameraPos = m_vHeadPos + vCameraOffset;
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);
        m_pTransformCom->LookAt(m_vHeadPos);
    }

}

void CCamera_Player::On_MouseMove(_float fTimeDelta)
{
    // === ȭ�� �߾� ��ǥ ��� ===
    RECT rc;
    GetClientRect(g_hWnd, &rc);
    POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

    // ���� ���콺 ��ǥ ��������
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    //// === ���콺�� â ���ο� �ִ��� Ȯ�� ===
    //if (ptMouse.x < 0 || ptMouse.x >= rc.right || ptMouse.y < 0 || ptMouse.y >= rc.bottom)
    //    return;

    // ���콺 �̵��� ��� (�߾� ����)
    _int iMouseMoveY = ptMouse.y - ptCenter.y;

    // Pitch �� ������Ʈ (���� ȸ��)
    m_fPitch -= iMouseMoveY * fTimeDelta * m_fMouseSensor;
    m_fPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, m_fPitch)); // ���� ȸ�� ����

    // ���콺�� �ٽ� �߾����� �̵�
    ClientToScreen(g_hWnd, &ptCenter);
    SetCursorPos(ptCenter.x, ptCenter.y);

    //// ���콺�� â ���ο� ���α� (ClipCursor ���)
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
