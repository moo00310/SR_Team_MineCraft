#include "Collider_Capsule.h"
#include "Collider_Cube.h"

CCollider_Capsule::CCollider_Capsule(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCollider(pGraphic_Device)
{
}

CCollider_Capsule::CCollider_Capsule(const CCollider_Capsule& Prototype)
    : CCollider(Prototype)
    , m_pVB(Prototype.m_pVB)
    , m_pIB(Prototype.m_pIB)
    , m_iNumVertices(Prototype.m_iNumVertices)
    , m_dwFVF(Prototype.m_dwFVF)
    , m_iStride(Prototype.m_iStride)
    , m_ePrimitiveType(Prototype.m_ePrimitiveType)
    , m_iNumPrimitive(Prototype.m_iNumPrimitive)
{
    memcpy(m_vPoint, Prototype.m_vPoint, sizeof(_float3) * 16);
    Safe_AddRef(m_pVB);
    Safe_AddRef(m_pIB);
}

HRESULT CCollider_Capsule::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCollider_Capsule::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    // �Ű������� ���� ����ü�� ��� ������ ����
    memcpy(&m_StateDesc, pArg, sizeof(COLLCAPSULE_DESC));
    m_pTransformCom = m_StateDesc.pTransformCom;

    if (!m_pTransformCom)
        return E_FAIL;

    Safe_AddRef(m_pTransformCom);

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

    // ĸ���� �� �� ���� �����ϴ� ������ ���
    _float3 vOffset = _float3(m_StateDesc.fOffsetX, m_StateDesc.fOffsetY, m_StateDesc.fOffsetZ);

    for (int i = 0; i < 8; ++i)
    {
        _float angle = D3DX_PI * 2 * (i / 8.0f);
        _float x = m_StateDesc.fRadius * cosf(angle);
        _float z = m_StateDesc.fRadius * sinf(angle);

        // ��� ��
        pVertices[i].vPosition = m_vPoint[i] = _float3(x, m_StateDesc.fHeight / 2.0f, z) + vOffset;
        // �ϴ� ��
        pVertices[i + 8].vPosition = m_vPoint[i + 8] = _float3(x, -m_StateDesc.fHeight / 2.0f, z) + vOffset;
    }
    // ĸ���� �ݱ� �κ��� ���� �߾� �� �߰�
    pVertices[16].vPosition = m_vPoint[16] = _float3(0, m_StateDesc.fHeight / 2.0f + m_StateDesc.fRadius, 0) + vOffset; // ����
    pVertices[17].vPosition = m_vPoint[17] = _float3(0, -m_StateDesc.fHeight / 2.0f - m_StateDesc.fRadius, 0) + vOffset; // �Ʒ���

    m_pVB->Unlock();

    m_iIndicesByte = sizeof(FACEINDICES16);
    m_eIndexFormat = D3DFMT_INDEX16;

    // �ε��� ���� ����
    if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iNumPrimitive * m_iIndicesByte, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
        return E_FAIL;

    FACEINDICES16* pIndices = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndices, 0);

    int index = 0;

    // ���� ����� �κ�
    for (int i = 0; i < 7; ++i)
    {
        pIndices[index++] = { static_cast<_ushort>(i), static_cast<_ushort>(i + 1), static_cast<_ushort>(i + 8) };
        pIndices[index++] = { static_cast<_ushort>(i + 1), static_cast<_ushort>(i + 9), static_cast<_ushort>(i + 8) };
    }
    // ������ ���� �ﰢ�� ����
    pIndices[index++] = { 7, 0, 15 };
    pIndices[index++] = { 0, 8, 15 };

    // ��� �ݱ�
    for (int i = 0; i < 7; ++i)
    {
        pIndices[index++] = { 16, static_cast<_ushort>(i), static_cast<_ushort>(i + 1) };
    }
    pIndices[index++] = { 16, 7, 0 };

    // �ϴ� �ݱ�
    for (int i = 0; i < 7; ++i)
    {
        pIndices[index++] = { 17, static_cast<_ushort>(i + 8), static_cast<_ushort>(i + 9) };
    }
    pIndices[index++] = { 17, 15, 8 };

    m_pIB->Unlock();

    return S_OK;
}




HRESULT CCollider_Capsule::Update_ColliderCapsule()
{
    const _float4x4* pWorldMatrix = m_pTransformCom->Get_WorldMatrix();
    _float4x4 StateMatrix = *pWorldMatrix;

    _float3 vecOffsetPos = *(_float3*)&(StateMatrix.m[3][0]);
    vecOffsetPos.x += m_StateDesc.fOffsetX;
    vecOffsetPos.y += m_StateDesc.fOffsetY;
    vecOffsetPos.z += m_StateDesc.fOffsetZ;

    StateMatrix.m[3][0] = vecOffsetPos.x;
    StateMatrix.m[3][1] = vecOffsetPos.y;
    StateMatrix.m[3][2] = vecOffsetPos.z;

    for (int i = 0; i < 16; ++i)
    {
        D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &StateMatrix);
    }

    return S_OK;
}

HRESULT CCollider_Capsule::Render_ColliderCapsule(_bool isHit)
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


_bool CCollider_Capsule::Collision_Check(CCollider_Capsule* pTarget, _Out_ _float3* pOutDistance, _Out_ COLLISION_DIR* pOutDir)
{
    // �浹 üũ ������ �ۼ��մϴ�.
    return false;
}

_bool CCollider_Capsule::Collision_Check_Cube(CCollider_Cube* pTarget, _float3* pOutDistance, COLLISION_DIR* pOutDir)
{
    if (pOutDistance)
        *pOutDistance = { 0.f, 0.f, 0.f };

    COLLISION_DIR Collision_Dir{ COLLISION_DIR::NONE };
    if (pOutDir)
        *pOutDir = Collision_Dir;

    if (pTarget == nullptr)
        return false;

    _float3 vMyPosition{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
    _float3 vTargetPosition{ pTarget->Get_Transform()->Get_State(CTransform::STATE_POSITION)};

    // AABB �ּ�/�ִ� ��ǥ ���
    _float3 minA = pTarget->GetMin();
    _float3 maxA = pTarget->GetMax();

    _float3 minB = pTarget->GetMin();
    _float3 maxB = pTarget->GetMax();

    // AABB �浹 �˻�
    if (maxA.x < minB.x || minA.x > maxB.x ||
        maxA.y < minB.y || minA.y > maxB.y ||
        maxA.z < minB.z || minA.z > maxB.z)
    {
        return false;
    }

    // �浹 ���� ���
    _float3 overlap = { min(maxA.x, maxB.x) - max(minA.x, minB.x),
                        min(maxA.y, maxB.y) - max(minA.y, minB.y),
                        min(maxA.z, maxB.z) - max(minA.z, minB.z) };

    if (pOutDistance)
        *pOutDistance = overlap;

    if (pOutDir)
    {
        if (overlap.y <= overlap.x && overlap.y <= overlap.z)
            Collision_Dir = (minA.y < minB.y) ? COLLISION_DIR::UP : COLLISION_DIR::DOWN;
        else if (overlap.x <= overlap.z)
            Collision_Dir = (minA.x < minB.x) ? COLLISION_DIR::RIGHT : COLLISION_DIR::LEFT;
        else
            Collision_Dir = (minA.z < minB.z) ? COLLISION_DIR::FRONT : COLLISION_DIR::BACK;

        *pOutDir = Collision_Dir;
    }

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
    Safe_Release(m_pVB);
    Safe_Release(m_pIB);
    Safe_Release(m_pTransformCom);

    CComponent::Free();
}
