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

	// ��� ��ȯ
	if (m_pGameInstance->Key_Down(VK_F5))
	{
		m_eCameraMode = (m_eCameraMode == E_CAMERA_MODE::FPS) ? E_CAMERA_MODE::TPS : E_CAMERA_MODE::FPS;
	}
}

void CCamera_Player::Late_Update(_float fTimeDelta)
{
	// â�� Ȱ��ȭ ���°� �ƴ� ��� ���콺 �Է��� ����
	if (!(GetForegroundWindow() == g_hWnd))
		return;

	// ȭ�� �߾� ��ǥ ���
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

	// ���� ���콺 ��ǥ ��������
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	// === ���콺�� â ���ο� �ִ��� Ȯ�� ===
	if (ptMouse.x < 0 || ptMouse.x >= rc.right || ptMouse.y < 0 || ptMouse.y >= rc.bottom)
		return;

	// ���콺 �̵��� ��� (�߾� ����)
	_int iMouseMoveX = ptMouse.x - ptCenter.x;
	_int iMouseMoveY = ptMouse.y - ptCenter.y;

	// ���콺 �̵����� ������� ī�޶� ȸ�� ����
	m_fYaw += iMouseMoveX * fTimeDelta * m_fMouseSensor;
	m_fPitch -= iMouseMoveY * fTimeDelta * m_fMouseSensor;

	// ��ġ(Pitch) �� ���� (���Ʒ� ȸ�� ���� ����)
	m_fPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, m_fPitch));

	// ���콺�� �ٽ� �߾����� �̵�
	ClientToScreen(g_hWnd, &ptCenter);
	SetCursorPos(ptCenter.x, ptCenter.y);

	// ���콺�� â ���ο� ���α� (ClipCursor ���)
	RECT clipRect;
	GetWindowRect(g_hWnd, &clipRect);
	ClipCursor(&clipRect);

	// ī�޶� ��忡 ���� ��ġ �� ���� ����
	_float3 vPlayerPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vLook;
	vLook.x = cosf(m_fPitch) * sinf(m_fYaw);
	vLook.y = sinf(m_fPitch);
	vLook.z = cosf(m_fPitch) * cosf(m_fYaw);

	if (m_eCameraMode == E_CAMERA_MODE::FPS)
	{
		// FPS ��� (1��Ī)
		_float3 vCameraPos = vPlayerPos + _float3(0.f, 1.8f, 0.f);  // �÷��̾� �Ӹ� ��ġ
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);
		m_pTransformCom->LookAt(vCameraPos + vLook);
	}
	else
	{
		// TPS ��� (3��Ī)
		_float3 vCameraOffset = -vLook * 5.0f;  // ĳ���Ϳ��� ���� �Ÿ� ����
		_float3 vCameraPos = vPlayerPos + vCameraOffset + _float3(0.f, 1.8f, 0.f); // ��¦ ����
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);
		m_pTransformCom->LookAt(vPlayerPos + _float3(0.f, 1.5f, 0.f)); // ĳ���� �Ӹ� �߽� �ٶ�
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
