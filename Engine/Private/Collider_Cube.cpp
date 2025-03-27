#include "..\Public\Collider_Cube.h"


CCollider_Cube::CCollider_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider(pGraphic_Device)
{

}

CCollider_Cube::CCollider_Cube(const CCollider_Cube& Prototype)
	: CCollider(Prototype)

{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
	memcpy(m_vPoint, Prototype.m_vPoint, sizeof(_float3) * 4);

}

HRESULT CCollider_Cube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider_Cube::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	memcpy(&m_StateDesc, pArg, sizeof(COLLCUBE_DESC));
	m_pTransformCom = m_StateDesc.pTransformCom;

	if (!m_pTransformCom)
		return E_FAIL;

	Safe_AddRef(m_pTransformCom);

	m_iNumVertices = 8;
	m_iStride = sizeof(VTXCUBETEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	/* ���� ���� ���� */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iStride, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, 0)))
		return E_FAIL;

	VTXCUBETEX* pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	// Offset ����
	_float3 vOffset = _float3(m_StateDesc.fOffSetX, m_StateDesc.fOffSetY, m_StateDesc.fOffsetZ);

	pVertices[0].vPosition = m_vPoint[0] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ) + vOffset;
	pVertices[1].vPosition = m_vPoint[1] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ) + vOffset;
	pVertices[2].vPosition = m_vPoint[2] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ) + vOffset;
	pVertices[3].vPosition = m_vPoint[3] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ) + vOffset;
	pVertices[4].vPosition = m_vPoint[4] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ) + vOffset;
	pVertices[5].vPosition = m_vPoint[5] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ) + vOffset;
	pVertices[6].vPosition = m_vPoint[6] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ) + vOffset;
	pVertices[7].vPosition = m_vPoint[7] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ) + vOffset;

	for (int i = 0; i < 8; i++)
		pVertices[i].vTexture = pVertices[i].vPosition;

	m_pVB->Unlock();

	m_iIndicesByte = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	/* �ε��� ���� ���� */
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iNumPrimitive * m_iIndicesByte, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	/* �ε��� ���� */
	pIndices[0] = { 1, 5, 6 }; pIndices[1] = { 1, 6, 2 };  // +X
	pIndices[2] = { 4, 0, 3 }; pIndices[3] = { 4, 3, 7 };  // -X
	pIndices[4] = { 4, 5, 1 }; pIndices[5] = { 4, 1, 0 };  // +Y
	pIndices[6] = { 3, 2, 6 }; pIndices[7] = { 3, 6, 7 };  // -Y
	pIndices[8] = { 5, 4, 7 }; pIndices[9] = { 5, 7, 6 };  // +Z
	pIndices[10] = { 0, 1, 2 }; pIndices[11] = { 0, 2, 3 };  // -Z

	m_pIB->Unlock();

	return S_OK;
}


HRESULT CCollider_Cube::Update_Collider()
{
	const _float4x4* pWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	_float4x4 StateMatrix = *pWorldMatrix;

	// �⺻ 8���� ������ ����
	m_vPoint[0] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[1] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[2] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[3] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[4] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);
	m_vPoint[5] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);
	m_vPoint[6] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);
	m_vPoint[7] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);

	// ���� ��ġ�� offset ����
	_float3 vecOffsetPos = *(_float3*)&(StateMatrix.m[3][0]);
	vecOffsetPos.x += m_StateDesc.fOffSetX;
	vecOffsetPos.y += m_StateDesc.fOffSetY;
	vecOffsetPos.z += m_StateDesc.fOffsetZ;

	// ���� ����� ��ġ �κ� ������Ʈ
	StateMatrix.m[3][0] = vecOffsetPos.x;
	StateMatrix.m[3][1] = vecOffsetPos.y;
	StateMatrix.m[3][2] = vecOffsetPos.z;

	// 8���� �������� ���� ��ķ� ��ȯ
	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &StateMatrix);
	}

	return S_OK;
}


HRESULT CCollider_Cube::Render_Collider(_bool isHit)
{
	// ���� ��ȯ ��� ����
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	// ���̾������� ��� ����
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// bRed�� true�� �� ������ ����
	if (isHit)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 0, 0)); // ������
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	}
	else
	{
		// ���� �ؽ�ó�� ����ϵ��� ���� ����
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}

	// ���� �� �ε��� ���� ����
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->SetIndices(m_pIB);

	// ������Ƽ��(����) �׸���
	m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	// ���� ����(�ָ��� ���)�� ����
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// ���� ���� - �ؽ�ó ���� ���¸� �⺻������ ����
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);

	// �ؽ�ó �ܰ� ����
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	return S_OK;

}

_bool CCollider_Cube::Collision_Check(CCollider_Cube* pTarget, _Out_ _float3* pOutDistance, _Out_ CCollider::COLLISION_DIR* pOutDir)
{
	if (pOutDistance)
		*pOutDistance = { 0.f, 0.f, 0.f };

	CCollider_Cube::COLLISION_DIR Collision_Dir{ CCollider_Cube::COLLISION_DIR::NONE };
	if (pOutDir)
		*pOutDir = Collision_Dir;

	if (pTarget == nullptr)
		return false;

	_float3 vMyPosition{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
	_float3 vTargetPosition{ pTarget->m_pTransformCom->Get_State(CTransform::STATE_POSITION) };

	// AABB �ּ�/�ִ� ��ǥ ���
	_float3 minA = {vMyPosition.x - m_StateDesc.fRadiusX,
					vMyPosition.y - m_StateDesc.fRadiusY,
					vMyPosition.z - m_StateDesc.fRadiusZ };

	_float3 maxA = {vMyPosition.x + m_StateDesc.fRadiusX,
					vMyPosition.y + m_StateDesc.fRadiusY,
					vMyPosition.z + m_StateDesc.fRadiusZ };

	_float3 minB = {vTargetPosition.x - pTarget->m_StateDesc.fRadiusX,
					vTargetPosition.y - pTarget->m_StateDesc.fRadiusY,
					vTargetPosition.z - pTarget->m_StateDesc.fRadiusZ };

	_float3 maxB = {vTargetPosition.x + pTarget->m_StateDesc.fRadiusX,
					vTargetPosition.y + pTarget->m_StateDesc.fRadiusY,
					vTargetPosition.z + pTarget->m_StateDesc.fRadiusZ };

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


// CCollider_Cube Ŭ������ min, max ���� �����ϴ� �Լ� �߰�
// AABB�� �ּҰ� ��ȯ
_float3 CCollider_Cube::GetMin() const
{
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// �ּ� ��ǥ ���
	return _float3(
		vPosition.x - m_StateDesc.fRadiusX,
		vPosition.y - m_StateDesc.fRadiusY,
		vPosition.z - m_StateDesc.fRadiusZ
	);
}

// AABB�� �ִ밪 ��ȯ
_float3 CCollider_Cube::GetMax() const
{
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// �ִ� ��ǥ ���
	return _float3(
		vPosition.x + m_StateDesc.fRadiusX,
		vPosition.y + m_StateDesc.fRadiusY,
		vPosition.z + m_StateDesc.fRadiusZ
	);
}

CCollider_Cube * CCollider_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device/*, COLLRECTDESC& Des*/)
{
	CCollider_Cube*	pInstance = new CCollider_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(/*Des*/)))
	{
		MSG_BOX("Failed to Created : CCollider_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_Cube::Clone(void * pArg)
{
 	CCollider_Cube*	pInstance = new CCollider_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCollider_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Cube::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pTransformCom);
}
