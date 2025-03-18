#include "Camera.h"
#include <DirectXMath.h>
using namespace DirectX;

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CCamera::CCamera(const CCamera& Prototype)
    : CGameObject( Prototype )
{
}

HRESULT CCamera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);
    
    if (nullptr == m_pTransformCom)
        return E_FAIL;

    /* 카메라의 초기상태를 트랜스폼에게 동기화한다. */
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, pDesc->vEye);
    m_pTransformCom->LookAt(pDesc->vAt);

    m_fFov = pDesc->fFov;    
    m_fNear = pDesc->fNear;
    m_fFar = pDesc->fFar;

    D3DVIEWPORT9            ViewportDesc{};
    m_pGraphic_Device->GetViewport(&ViewportDesc);

    m_fAspect = static_cast<_float>(ViewportDesc.Width) / ViewportDesc.Height;    

    return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
    if (!m_isActive)
        return;
}

void CCamera::Update(_float fTimeDelta)
{
    if (!m_isActive)
        return;
}

void CCamera::Late_Update(_float fTimeDelta)
{
    if (!m_isActive)
        return;
}

HRESULT CCamera::Render()
{
    if (!m_isActive)
        return S_OK;

    return S_OK;
}

void CCamera::Set_Active(_bool _isActive)
{
    m_isActive = _isActive;
}

const _float3& CCamera::Get_LookAt()
{
    return m_pTransformCom->Get_State(CTransform::STATE_LOOK);
}

void CCamera::Set_LookAt(const _float3& vLookAt)
{
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLookAt);
}

CTransform* CCamera::Get_Transform()
{
    return m_pTransformCom;
}

_float CCamera::Get_Yaw()
{
    _float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

    // Look 벡터에서 Yaw 계산 (XZ 평면 기준)
    return atan2f(vLook.x, vLook.z);
}

_float CCamera::Get_Pitch()
{
    _float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

    // Look 벡터에서 Pitch 계산 (위아래 각도)
    return asinf(vLook.y);
}

void CCamera::Set_Yaw(const _float fYaw)
{
    const _float fPitch = Get_Pitch(); // 기존 Pitch 값 유지

    DirectX::XMFLOAT3 vLook; // 최신 자료형 XMFLOAT3 사용
    vLook.x = cosf(fPitch) * sinf(fYaw);
    vLook.y = sinf(fPitch);
    vLook.z = cosf(fPitch) * cosf(fYaw);

    // Look 벡터 정규화 후 적용
    XMVECTOR vLookVector = XMLoadFloat3(&vLook);  // XMFLOAT3 -> XMVECTOR 변환
    vLookVector = XMVector3Normalize(vLookVector);

    // 정규화된 벡터를 XMFLOAT3로 다시 저장
    XMStoreFloat3(&vLook, vLookVector);

    // 변환된 값을 예전 자료형인 _float3로 변환 후 Set_State에 전달
    _float3 oldLook = { _float(vLook.x), _float(vLook.y), _float(vLook.z) };
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, oldLook);
}

void CCamera::Set_Pitch(const _float fPitch)
{
    const _float fYaw = Get_Yaw();  // 기존 Yaw 값 유지

    // Pitch 제한 (고개가 너무 꺾이지 않도록)
    _float clampedPitch = max(-XM_PIDIV2 + 0.1f, min(XM_PIDIV2 - 0.1f, fPitch));

    DirectX::XMFLOAT3 vLook; // 최신 자료형 XMFLOAT3 사용
    vLook.x = cosf(clampedPitch) * sinf(fYaw);
    vLook.y = sinf(clampedPitch);
    vLook.z = cosf(clampedPitch) * cosf(fYaw);

    // Look 벡터 정규화 후 적용
    XMVECTOR vLookVector = XMLoadFloat3(&vLook);  // XMFLOAT3 -> XMVECTOR 변환
    vLookVector = XMVector3Normalize(vLookVector);

    // 정규화된 벡터를 XMFLOAT3로 다시 저장
    XMStoreFloat3(&vLook, vLookVector);

    // 변환된 값을 예전 자료형인 _float3로 변환 후 Set_State에 전달
    _float3 oldLook = { _float(vLook.x), _float(vLook.y), _float(vLook.z) };
    m_pTransformCom->Set_State(CTransform::STATE_LOOK, oldLook);
}

HRESULT CCamera::Update_VP_Matrices()
{
    _float4x4       ViewMatrix{}, ProjMatrix{};

    m_pGraphic_Device->SetTransform(D3DTS_VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());

    m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&ProjMatrix, m_fFov, m_fAspect, m_fNear, m_fFar));


    return S_OK;
}

void CCamera::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
}
