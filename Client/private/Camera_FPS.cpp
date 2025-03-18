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

	/* TransformCom�� �����س��´�. */
	if (FAILED(Ready_Components()))
		return E_FAIL;

	//�ƱԸ�Ʈ �ޱ�
	CAMERA_FPS_DESC	Desc{ *static_cast<CAMERA_FPS_DESC*>(pArg) };
	m_fMouseSensor = Desc.fMouseSensor;
	if (!Desc.pTarget)
		return E_FAIL;
	//�÷��̾� Ʈ������ �ޱ�
	m_pTargetTransformCom = static_cast<CTransform*>(Desc.pTarget->Find_Component(TEXT("Com_Transform")));


	//ī�޶� �⺻ �� ����
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	//���콺 �õ尪 ����
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

    // ���� â�� Ȱ��ȭ �������� Ȯ��
    bool isActive = (GetForegroundWindow() == g_hWnd);

    // 1. ȭ�� �߾� ��ǥ ���
    RECT rc;
    GetClientRect(g_hWnd, &rc);
    POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

    // 2. ���� ���콺 ��ǥ ��������
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse); // Ŭ���̾�Ʈ ��ǥ�� ��ȯ

    // 3. â�� Ȱ��ȭ ���°� �ƴ� ��� ���콺 �Է��� ����
    if (!isActive)
        return;

    // === ���콺�� â ���ο� �ִ��� Ȯ�� ===
    if (ptMouse.x < 0 || ptMouse.x >= rc.right || ptMouse.y < 0 || ptMouse.y >= rc.bottom)
        return;  // â ���̸� ȸ�� ������ �������� ����

    // 4. ���콺 �̵��� ��� (�߾� ����)
    _int iMouseMoveX = ptMouse.x - ptCenter.x;
    _int iMouseMoveY = ptMouse.y - ptCenter.y;

    // 5. ���콺 �̵����� ������� ī�޶� ȸ�� ����
    _float fYawDelta = iMouseMoveX * fTimeDelta * m_fMouseSensor;
    _float fPitchDelta = iMouseMoveY * fTimeDelta * m_fMouseSensor;

    // ���� ī�޶��� look ������ _float3�� ������
    _float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

    // ��ġ(Pitch) �� ���� (���Ʒ� ȸ�� ���� ����)
    fPitchDelta = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, fPitchDelta));

    // ȸ�� ���: yaw�� pitch�� ���� look ���� ���
    _float fYaw = atan2f(vLook.z, vLook.x) + fYawDelta;  // ���� yaw�� delta �߰�
    _float fPitch = asinf(vLook.y) + fPitchDelta;  // ���� pitch�� delta �߰�

    // �� look ���� ���
    vLook.x = cosf(fPitch) * sinf(fYaw);
    vLook.y = sinf(fPitch);
    vLook.z = cosf(fPitch) * cosf(fYaw);

    // Look ���� ����ȭ
    XMVECTOR vLookVector = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&vLook));
    vLookVector = XMVector3Normalize(vLookVector);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&vLook), vLookVector);

    // ī�޶� ȸ�� ���� (Look ���� ����)
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook);

    // 6. ���콺�� �ٽ� �߾����� �̵�
    ClientToScreen(g_hWnd, &ptCenter);
    SetCursorPos(ptCenter.x, ptCenter.y);

    // 7. ���콺�� â ���ο� ���α� (ClipCursor ���)
    RECT clipRect;
    GetWindowRect(g_hWnd, &clipRect);
    ClipCursor(&clipRect); // ���콺�� â ���ο� ����

    // 8. FPS ī�޶��� ��ġ ���� (�÷��̾��� �Ӹ� ��ġ)
    _float3 vPlayerPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION);
    _float3 vCameraPos = vPlayerPos + _float3(0.f, 1.8f, 0.f);  // �÷��̾� �Ӹ� ��ġ

    // 9. ī�޶� ��ġ ����
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);  // ī�޶� ��ġ ����
    m_pTransformCom->LookAt(vCameraPos + vLook);  // 1��Ī ī�޶�� �ڽ��� ���� �ٶ�

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
