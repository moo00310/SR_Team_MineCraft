#include "FrustumCulling_Manager.h"

CFrustumCulling_Manager::CFrustumCulling_Manager(LPDIRECT3DDEVICE9 pGraphicDevice)
	:m_pGraphicDevice(pGraphicDevice)
{
	Safe_AddRef(m_pGraphicDevice);
}

HRESULT CFrustumCulling_Manager::Initilize(LPDIRECT3DDEVICE9 pGraphicDevice)
{
    // 뷰 공간에서의 프러스텀 정점 설정 (NDC 좌표계 기준, 정규화된 좌표)
    m_WorldPoint[0] = _float3(-1.f, 1.f, 0.f);
    m_WorldPoint[1] = _float3(1.f, 1.f, 0.f);
    m_WorldPoint[2] = _float3(1.f, -1.f, 0.f);
    m_WorldPoint[3] = _float3(-1.f, -1.f, 0.f);
    m_WorldPoint[4] = _float3(-1.f, 1.f, 1.f);
    m_WorldPoint[5] = _float3(1.f, 1.f, 1.f);
    m_WorldPoint[6] = _float3(1.f, -1.f, 1.f);
    m_WorldPoint[7] = _float3(-1.f, -1.f, 1.f);

    return S_OK;
}

HRESULT CFrustumCulling_Manager::Update()
{
    // 프러스텀 정점을 다시 설정 (NDC 기준)
    m_WorldPoint[0] = _float3(-1.f, 1.f, 0.f);
    m_WorldPoint[1] = _float3(1.f, 1.f, 0.f);
    m_WorldPoint[2] = _float3(1.f, -1.f, 0.f);
    m_WorldPoint[3] = _float3(-1.f, -1.f, 0.f);
    m_WorldPoint[4] = _float3(-1.f, 1.f, 1.f);
    m_WorldPoint[5] = _float3(1.f, 1.f, 1.f);
    m_WorldPoint[6] = _float3(1.f, -1.f, 1.f);
    m_WorldPoint[7] = _float3(-1.f, -1.f, 1.f);

    // 프로젝션 행렬의 역행렬을 구하여 클립 공간에서 뷰 공간으로 변환
    _float4x4 ProjMatrixInv;
    m_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &ProjMatrixInv);
    D3DXMatrixInverse(&ProjMatrixInv, nullptr, &ProjMatrixInv);

    // 클립 공간의 정점들을 뷰 공간으로 변환
    for (int i = 0; i < 8; ++i)
        D3DXVec3TransformCoord(&m_WorldPoint[i], &m_WorldPoint[i], &ProjMatrixInv);

    // 뷰 행렬의 역행렬을 구하여 뷰 공간에서 월드 공간으로 변환
    _float4x4 ViewMatrixInv;
    m_pGraphicDevice->GetTransform(D3DTS_VIEW, &ViewMatrixInv);
    D3DXMatrixInverse(&ViewMatrixInv, nullptr, &ViewMatrixInv);

    // 뷰 공간의 정점들을 월드 공간으로 변환
    for (int i = 0; i < 8; ++i)
        D3DXVec3TransformCoord(&m_WorldPoint[i], &m_WorldPoint[i], &ViewMatrixInv);

    return S_OK;
}

_bool CFrustumCulling_Manager::Is_In_Frustum(_float3 vPos, _float fRadius)
{
    D3DXPlaneFromPoints(&m_FrustumPlane[0], &m_WorldPoint[0], &m_WorldPoint[1], &m_WorldPoint[2]);
    D3DXPlaneFromPoints(&m_FrustumPlane[1], &m_WorldPoint[1], &m_WorldPoint[5], &m_WorldPoint[6]);
    D3DXPlaneFromPoints(&m_FrustumPlane[2], &m_WorldPoint[4], &m_WorldPoint[5], &m_WorldPoint[1]);
    D3DXPlaneFromPoints(&m_FrustumPlane[3], &m_WorldPoint[4], &m_WorldPoint[0], &m_WorldPoint[3]);
    D3DXPlaneFromPoints(&m_FrustumPlane[4], &m_WorldPoint[5], &m_WorldPoint[4], &m_WorldPoint[7]);
    D3DXPlaneFromPoints(&m_FrustumPlane[5], &m_WorldPoint[3], &m_WorldPoint[2], &m_WorldPoint[6]);

    for (int i = 0; i < 6; ++i)
    {
        _float distance = m_FrustumPlane[i].a * vPos.x + m_FrustumPlane[i].b * vPos.y + m_FrustumPlane[i].c * vPos.z + m_FrustumPlane[i].d;
        if (distance > fRadius + 1)
            return false;
    }

    return true;
}

CFrustumCulling_Manager* CFrustumCulling_Manager::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CFrustumCulling_Manager* pInstance = new CFrustumCulling_Manager(pGraphicDevice);
    if (FAILED(pInstance->Initilize(pGraphicDevice)))
    {
		Safe_Release(pInstance);
		MSG_BOX("Failed to Create CFrustumCulling_Manager");
    }

    return pInstance;
}

void CFrustumCulling_Manager::Free()
{
	Safe_Release(m_pGraphicDevice);
}
