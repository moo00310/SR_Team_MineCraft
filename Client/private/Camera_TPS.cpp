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

	/* TransformCom�� �����س��´�. */
	if (FAILED(Ready_Components()))
		return E_FAIL;

	//�ƱԸ�Ʈ �ޱ�
	CAMERA_TPS_DESC	Desc{ *static_cast<CAMERA_TPS_DESC*>(pArg) };
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
    m_fYaw += iMouseMoveX * fTimeDelta * m_fMouseSensor;
    m_fPitch -= iMouseMoveY * fTimeDelta * m_fMouseSensor;

    // ��ġ(Pitch) �� ���� (���Ʒ� ȸ�� ���� ����)
    m_fPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, m_fPitch));

    // 6. ���콺�� �ٽ� �߾����� �̵�
    ClientToScreen(g_hWnd, &ptCenter);
    SetCursorPos(ptCenter.x, ptCenter.y);

    // 7. ���콺�� â ���ο� ���α� (ClipCursor ���)
    RECT clipRect;
    GetWindowRect(g_hWnd, &clipRect);
    ClipCursor(&clipRect); // ���콺�� â ���ο� ����

    // 8. ĳ���� �Ӹ� ��ġ �������� ȸ�� �߽� ����
    _float3 vTargetPos = m_pTargetTransformCom->Get_State(CTransform::STATE_POSITION);
    _float fHeadHeight = 1.5f;  // ĳ���� �Ӹ� ���� ����
    _float3 vHeadPos = vTargetPos + _float3(0.f, fHeadHeight, 0.f);  // �Ӹ� ��ġ

    // 9. ī�޶� ���� ���� ��� (1��Ī�� ������ ���)
    _float3 vLook;
    vLook.x = cosf(m_fPitch) * sinf(m_fYaw);
    vLook.y = sinf(m_fPitch);
    vLook.z = cosf(m_fPitch) * cosf(m_fYaw);
    XMVECTOR vLookVector = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&vLook));
    vLookVector = XMVector3Normalize(vLookVector);
    XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&vLook), vLookVector);

    // 10. �Ӹ��� �߽����� ī�޶� ��ġ (ĳ���� �Ӹ����� fRadius��ŭ �ڷ� ��ġ)
    _float fRadius = 3.5f;  // ī�޶� �Ÿ�
    _float3 vCameraPos = vHeadPos - vLook * fRadius;

    // 11. ī�޶� ��ġ�� ���� ����
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);
    m_pTransformCom->LookAt(vHeadPos);  // �Ӹ��� �ٶ󺸰� ����

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
