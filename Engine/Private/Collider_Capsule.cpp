#include "Collider_Capsule.h"
#include "Collider_Cube.h"

CCollider_Capsule::CCollider_Capsule(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCollider(pGraphic_Device)
{
}

CCollider_Capsule::CCollider_Capsule(const CCollider_Capsule& Prototype)
    : CCollider(Prototype)

{
    Safe_AddRef(m_pVB);
    Safe_AddRef(m_pIB);
    memcpy(m_vPoint, Prototype.m_vPoint, sizeof(_float3) * 16);

}

HRESULT CCollider_Capsule::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCollider_Capsule::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    COLLCAPSULE_DESC tDesc;

    // 매개변수로 받은 구조체를 멤버 변수에 복사
    memcpy(&tDesc, pArg, sizeof(COLLCAPSULE_DESC));

    m_fRadius = tDesc.fRadius;
    m_fHeight = tDesc.fHeight;
    m_vOffset = tDesc.vfOffset;

    m_pTransformCom = tDesc.pTransformCom;
    m_pOwner = tDesc.pOwner;

    if (!m_pTransformCom)
        return E_FAIL;

    if(!m_pOwner)
        return E_FAIL;

    Safe_AddRef(m_pTransformCom);
    Safe_AddRef(m_pOwner);

    m_iNumVertices = 18; // 원기둥의 원 둘레 8개 + 위아래 중앙점 2개
    m_iStride = sizeof(VTXCUBETEX); // 정점 구조체 크기
    m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
    m_ePrimitiveType = D3DPT_TRIANGLELIST;
    m_iNumPrimitive = 32; // 삼각형 개수

    // 정점 버퍼 생성
    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iStride, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, 0)))
        return E_FAIL;

    VTXCUBETEX* pVertices = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertices, 0);

    // 캡슐의 중심을 기준으로 반지름과 높이를 설정
    float radius = m_fRadius;
    float fHeight = m_fHeight;

    // 원기둥의 중간을 생성 (옆으로 펼쳐진 원)
    for (int i = 0; i < 8; ++i) {
        float angle = (i * D3DX_PI * 2.0f) / 8;
        pVertices[i].vPosition = D3DXVECTOR3(radius * cos(angle), 0, radius * sin(angle)) + m_vOffset;
        //pVertices[i].vTex = D3DXVECTOR2(cos(angle), sin(angle)); // 텍스처 좌표 (각도 기반)
    }

    // 캡슐의 위쪽과 아래쪽 반구를 추가
    for (int i = 8; i < 10; ++i) {
        pVertices[i].vPosition = D3DXVECTOR3(0, (i == 8 ? fHeight / 2.0f : -fHeight / 2.0f), 0) + m_vOffset;
        //pVertices[i].vTex = D3DXVECTOR2(0.5f, 0.5f); // 중앙 위치
    }

    m_pVB->Unlock();

    m_iIndicesByte = sizeof(FACEINDICES16);
    m_eIndexFormat = D3DFMT_INDEX16;

    // 인덱스 버퍼 생성
    if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iNumPrimitive * m_iIndicesByte, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
        return E_FAIL;

    FACEINDICES16* pIndices = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndices, 0);

    // 원기둥의 몸통 부분의 인덱스 설정
    for (int i = 0; i < 8; ++i) {
        int next = (i + 1) % 8;
        pIndices[i * 6 + 0] = { (unsigned short)i, (unsigned short)next, 8 }; // 원기둥 +Y면
        pIndices[i * 6 + 1] = { (unsigned short)next, (unsigned short)i, 9 }; // 원기둥 -Y면
    }

    // 위쪽 반구와 아래쪽 반구의 삼각형 인덱스 설정
    // 위쪽 반구
    for (int i = 0; i < 8; ++i) {
        int next = (i + 1) % 8;
        pIndices[i * 6 + 16] = { (unsigned short)i, 8, (unsigned short)next }; // 위쪽 반구
    }

    // 아래쪽 반구
    for (int i = 0; i < 8; ++i) {
        int next = (i + 1) % 8;
        pIndices[i * 6 + 24] = { (unsigned short)i, 9, (unsigned short)next }; // 아래쪽 반구
    }

    m_pIB->Unlock();

    return S_OK;
}



HRESULT CCollider_Capsule::Update_Collider()
{
    const _float4x4* pWorldMatrix = m_pTransformCom->Get_WorldMatrix();
    _float4x4 StateMatrix = *pWorldMatrix;

    _float3 vecOffsetPos = *(_float3*)&(StateMatrix.m[3][0]);

    vecOffsetPos += m_vOffset;


    StateMatrix.m[3][0] = vecOffsetPos.x;
    StateMatrix.m[3][1] = vecOffsetPos.y;
    StateMatrix.m[3][2] = vecOffsetPos.z;

    for (int i = 0; i < 16; ++i)
    {
        D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &StateMatrix);
    }

    return S_OK;
}

HRESULT CCollider_Capsule::Render_Collider(_bool isHit)
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    // 면을 그리기 위해 Solid 모드로 설정
    m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    if (isHit)
    {
        // 충돌 상태일 때 빨간색으로 설정
        m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 0, 0));
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
    }
    else
    {
        // 충돌 상태가 아닐 때 일반 색상으로 설정
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }

    // 버텍스 버퍼 설정
    m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
    m_pGraphic_Device->SetFVF(m_dwFVF);

    // 인덱스 버퍼 설정
    m_pGraphic_Device->SetIndices(m_pIB);

    // 인덱스 기반으로 그리기
    m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

    // 렌더 상태 초기화
    m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphic_Device->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


    return S_OK;
}


_bool CCollider_Capsule::Collision_Check(CCollider_Cube* pTarget, _Out_ _float3* pOutDepth, _Out_ CCollider::COLLISION_DIR* pOutDir)
{
    if (!pTarget)
        return false;

    if (pOutDepth)
        *pOutDepth = { 0.f, 0.f, 0.f };

    COLLISION_DIR Collision_Dir = COLLISION_DIR::NONE;
    if (pOutDir)
        *pOutDir = Collision_Dir;

    // 캡슐의 중심 위치 (오프셋을 고려하여 계산)
    _float3 vCapsuleCenter = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    vCapsuleCenter += m_vOffset;

    // 큐브의 월드 변환 행렬을 가져와 큐브의 월드 좌표 계산
    _float3 vCubeCenter = pTarget->Get_Transform()->Get_State(CTransform::STATE_POSITION);

    // 큐브의 오프셋을 적용 (오프셋은 위치에 더하는 형태)
    vCubeCenter += pTarget->Get_Offset();


    // AABB의 최소/최대 좌표 계산
    _float3 vTargetMin = pTarget->GetMin();
    _float3 vTargetMax = pTarget->GetMax();

    // AABB에 오프셋 적용 (큐브의 월드 좌표에 오프셋을 더한 후)
    vTargetMin += pTarget->Get_Offset();
    vTargetMax += pTarget->Get_Offset();


    // AABB의 월드 좌표를 계산 (오프셋을 고려하여 월드 공간에 적용)
    const _float4x4* matWorld = pTarget->Get_Transform()->Get_WorldMatrix();
    D3DXVec3TransformCoord(&vTargetMin, &vTargetMin, matWorld);
    D3DXVec3TransformCoord(&vTargetMax, &vTargetMax, matWorld);

    // AABB에서 캡슐의 원형 부분에 가장 가까운 점 찾기
    _float3 vClosestPoint;
    vClosestPoint.x = max(vTargetMin.x, min(vCapsuleCenter.x, vTargetMax.x));
    vClosestPoint.y = max(vTargetMin.y, min(vCapsuleCenter.y, vTargetMax.y));
    vClosestPoint.z = max(vTargetMin.z, min(vCapsuleCenter.z, vTargetMax.z));

    // 캡슐의 원형 부분과의 거리 계산
    _float3 vDiff = vClosestPoint - vCapsuleCenter;
    float fDistanceSquared = vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z;
    float fCapsuleRadiusSquared = m_fRadius * m_fRadius;

    // 충돌 여부 확인
    if (fDistanceSquared > fCapsuleRadiusSquared)
        return false; // 충돌 없음

    // 충돌 방향 계산
    if (pOutDir)
    {
        if (fabs(vDiff.y) >= fabs(vDiff.x) && fabs(vDiff.y) >= fabs(vDiff.z))
            Collision_Dir = (vDiff.y > 0) ? COLLISION_DIR::DOWN : COLLISION_DIR::UP;
        else if (fabs(vDiff.x) >= fabs(vDiff.z))
            Collision_Dir = (vDiff.x > 0) ? COLLISION_DIR::LEFT : COLLISION_DIR::RIGHT;
        else
            Collision_Dir = (vDiff.z > 0) ? COLLISION_DIR::FRONT : COLLISION_DIR::BACK;

        *pOutDir = Collision_Dir;
    }

    // 충돌 발생 시 깊이 계산
    float fDistance = sqrt(fDistanceSquared); // 거리 계산
    float fDepth = m_fRadius - fDistance; // 충돌 깊이 계산

    // 충돌 깊이 벡터 계산
    _float3 vDepthVec = vDiff;
    D3DXVec3Normalize(&vDepthVec, &vDepthVec); // 깊이 벡터를 정규화
    vDepthVec *= fDepth; // 깊이를 벡터에 곱해줌

    // 충돌 깊이를 pOutDepth에 저장
    if (pOutDepth)
        *pOutDepth = vDepthVec;

    /*if (pOutDepth)
        *pOutDepth = vDiff;*/

    return true;
}


CCollider_Capsule* CCollider_Capsule::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCollider_Capsule* pInstance = new CCollider_Capsule(pGraphic_Device);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MessageBox(nullptr, L"Failed to create Capsule Collider", nullptr, MB_OK);
        Safe_Release(pInstance);
    }
    return pInstance;
}

CComponent* CCollider_Capsule::Clone(void* pArg)
{
    CCollider_Capsule* pClone = new CCollider_Capsule(*this);

    if (pArg != nullptr)
    {
        pClone->Initialize(pArg);
    }

    return pClone;
}

void CCollider_Capsule::Free()
{
    __super::Free();

    Safe_Release(m_pVB);
    Safe_Release(m_pIB);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pOwner);
}
