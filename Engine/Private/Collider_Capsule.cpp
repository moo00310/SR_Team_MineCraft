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

    // �Ű������� ���� ����ü�� ��� ������ ����
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

    m_iNumVertices = 18; // ������� �� �ѷ� 8�� + ���Ʒ� �߾��� 2��
    m_iStride = sizeof(VTXCUBETEX); // ���� ����ü ũ��
    m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
    m_ePrimitiveType = D3DPT_TRIANGLELIST;
    m_iNumPrimitive = 32; // �ﰢ�� ����

    // ���� ���� ����
    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iStride, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, 0)))
        return E_FAIL;

    VTXCUBETEX* pVertices = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertices, 0);

    // ĸ���� �߽��� �������� �������� ���̸� ����
    float radius = m_fRadius;
    float fHeight = m_fHeight;

    // ������� �߰��� ���� (������ ������ ��)
    for (int i = 0; i < 8; ++i) {
        float angle = (i * D3DX_PI * 2.0f) / 8;
        pVertices[i].vPosition = D3DXVECTOR3(radius * cos(angle), 0, radius * sin(angle)) + m_vOffset;
        //pVertices[i].vTex = D3DXVECTOR2(cos(angle), sin(angle)); // �ؽ�ó ��ǥ (���� ���)
    }

    // ĸ���� ���ʰ� �Ʒ��� �ݱ��� �߰�
    for (int i = 8; i < 10; ++i) {
        pVertices[i].vPosition = D3DXVECTOR3(0, (i == 8 ? fHeight / 2.0f : -fHeight / 2.0f), 0) + m_vOffset;
        //pVertices[i].vTex = D3DXVECTOR2(0.5f, 0.5f); // �߾� ��ġ
    }

    m_pVB->Unlock();

    m_iIndicesByte = sizeof(FACEINDICES16);
    m_eIndexFormat = D3DFMT_INDEX16;

    // �ε��� ���� ����
    if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iNumPrimitive * m_iIndicesByte, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
        return E_FAIL;

    FACEINDICES16* pIndices = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndices, 0);

    // ������� ���� �κ��� �ε��� ����
    for (int i = 0; i < 8; ++i) {
        int next = (i + 1) % 8;
        pIndices[i * 6 + 0] = { (unsigned short)i, (unsigned short)next, 8 }; // ����� +Y��
        pIndices[i * 6 + 1] = { (unsigned short)next, (unsigned short)i, 9 }; // ����� -Y��
    }

    // ���� �ݱ��� �Ʒ��� �ݱ��� �ﰢ�� �ε��� ����
    // ���� �ݱ�
    for (int i = 0; i < 8; ++i) {
        int next = (i + 1) % 8;
        pIndices[i * 6 + 16] = { (unsigned short)i, 8, (unsigned short)next }; // ���� �ݱ�
    }

    // �Ʒ��� �ݱ�
    for (int i = 0; i < 8; ++i) {
        int next = (i + 1) % 8;
        pIndices[i * 6 + 24] = { (unsigned short)i, 9, (unsigned short)next }; // �Ʒ��� �ݱ�
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

    // ���� �׸��� ���� Solid ���� ����
    m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    if (isHit)
    {
        // �浹 ������ �� ���������� ����
        m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 0, 0));
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
    }
    else
    {
        // �浹 ���°� �ƴ� �� �Ϲ� �������� ����
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }

    // ���ؽ� ���� ����
    m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
    m_pGraphic_Device->SetFVF(m_dwFVF);

    // �ε��� ���� ����
    m_pGraphic_Device->SetIndices(m_pIB);

    // �ε��� ������� �׸���
    m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

    // ���� ���� �ʱ�ȭ
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

    // ĸ���� �߽� ��ġ (�������� ����Ͽ� ���)
    _float3 vCapsuleCenter = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    vCapsuleCenter += m_vOffset;

    // ť���� ���� ��ȯ ����� ������ ť���� ���� ��ǥ ���
    _float3 vCubeCenter = pTarget->Get_Transform()->Get_State(CTransform::STATE_POSITION);

    // ť���� �������� ���� (�������� ��ġ�� ���ϴ� ����)
    vCubeCenter += pTarget->Get_Offset();


    // AABB�� �ּ�/�ִ� ��ǥ ���
    _float3 vTargetMin = pTarget->GetMin();
    _float3 vTargetMax = pTarget->GetMax();

    // AABB�� ������ ���� (ť���� ���� ��ǥ�� �������� ���� ��)
    vTargetMin += pTarget->Get_Offset();
    vTargetMax += pTarget->Get_Offset();


    // AABB�� ���� ��ǥ�� ��� (�������� ����Ͽ� ���� ������ ����)
    const _float4x4* matWorld = pTarget->Get_Transform()->Get_WorldMatrix();
    D3DXVec3TransformCoord(&vTargetMin, &vTargetMin, matWorld);
    D3DXVec3TransformCoord(&vTargetMax, &vTargetMax, matWorld);

    // AABB���� ĸ���� ���� �κп� ���� ����� �� ã��
    _float3 vClosestPoint;
    vClosestPoint.x = max(vTargetMin.x, min(vCapsuleCenter.x, vTargetMax.x));
    vClosestPoint.y = max(vTargetMin.y, min(vCapsuleCenter.y, vTargetMax.y));
    vClosestPoint.z = max(vTargetMin.z, min(vCapsuleCenter.z, vTargetMax.z));

    // ĸ���� ���� �κа��� �Ÿ� ���
    _float3 vDiff = vClosestPoint - vCapsuleCenter;
    float fDistanceSquared = vDiff.x * vDiff.x + vDiff.y * vDiff.y + vDiff.z * vDiff.z;
    float fCapsuleRadiusSquared = m_fRadius * m_fRadius;

    // �浹 ���� Ȯ��
    if (fDistanceSquared > fCapsuleRadiusSquared)
        return false; // �浹 ����

    // �浹 ���� ���
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

    // �浹 �߻� �� ���� ���
    float fDistance = sqrt(fDistanceSquared); // �Ÿ� ���
    float fDepth = m_fRadius - fDistance; // �浹 ���� ���

    // �浹 ���� ���� ���
    _float3 vDepthVec = vDiff;
    D3DXVec3Normalize(&vDepthVec, &vDepthVec); // ���� ���͸� ����ȭ
    vDepthVec *= fDepth; // ���̸� ���Ϳ� ������

    // �浹 ���̸� pOutDepth�� ����
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
