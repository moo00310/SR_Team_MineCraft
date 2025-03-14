#include "..\Public\Collider_Cube.h"


//const _tchar* CCollider_Cube::m_pTransformTag = TEXT("Com_Transform");

//_bool m_bRender = true;

CCollider_Cube::CCollider_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CCollider_Cube::CCollider_Cube(const CCollider_Cube& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_dwFVF(rhs.m_dwFVF)
	, m_iStride(rhs.m_iStride)
	, m_ePrimitiveType(rhs.m_ePrimitiveType)
	, m_iNumPrimitive(rhs.m_iNumPrimitive)
{
	//m_eIndexFormat = {};
	memcpy(m_vPoint, rhs.m_vPoint, sizeof(_float3) * 4);
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CCollider_Cube::Initialize_Prototype(/*COLLRECTDESC& Des*/)
{
	//m_StateDesc = Des;

	//D3DXMatrixIdentity(&m_StateDesc.StateMatrix);

	//m_iNumVertices = 8;
	//m_iStride = sizeof(VTXCUBETEX);
	//m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	//m_ePrimitiveType = D3DPT_TRIANGLELIST;
	//m_iNumPrimitive = 12;

	///* �������� �Ҵ��ߴ�. */
	//if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iStride, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, 0)))
	//	return E_FAIL;

	//VTXCUBETEX*			pVertices = nullptr;

	//m_pVB->Lock(0, /*m_iNumVertices * m_iStride*/0, (void**)&pVertices, 0);

	//pVertices[0].vPosition = m_vPoint[0] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);// + InitPos;
	//pVertices[0].vTexture = pVertices[0].vPosition;

	//pVertices[1].vPosition = m_vPoint[1] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);// +InitPos;
	//pVertices[1].vTexture = pVertices[1].vPosition;

	//pVertices[2].vPosition = m_vPoint[2] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);// +InitPos;
	//pVertices[2].vTexture = pVertices[2].vPosition;

	//pVertices[3].vPosition = m_vPoint[3] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);// +InitPos;
	//pVertices[3].vTexture = pVertices[3].vPosition;

	//pVertices[4].vPosition = m_vPoint[4] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);// + InitPos;
	//pVertices[4].vTexture = pVertices[4].vPosition;

	//pVertices[5].vPosition = m_vPoint[5] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);// +InitPos;
	//pVertices[5].vTexture = pVertices[5].vPosition;

	//pVertices[6].vPosition = m_vPoint[6] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);// +InitPos;
	//pVertices[6].vTexture = pVertices[6].vPosition;

	//pVertices[7].vPosition = m_vPoint[7] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);// +InitPos;
	//pVertices[7].vTexture = pVertices[7].vPosition;
	//m_pVB->Unlock();

	//m_iIndicesByte = sizeof(FACEINDICES16);
	//m_eIndexFormat = D3DFMT_INDEX16;

	//if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iNumPrimitive * m_iIndicesByte, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
	//	return E_FAIL;

	//FACEINDICES16*			pIndices = nullptr;

	//m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	///* +X */
	//pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
	//pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

	///* -X */
	//pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
	//pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

	///* +Y */
	//pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
	//pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

	///* -Y */
	//pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
	//pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

	///* +Z */
	//pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
	//pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

	///* -Z */
	//pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
	//pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;

	//m_pIB->Unlock();
	
	return S_OK;
}

HRESULT CCollider_Cube::Initialize(void * pArg)
{
	//m_pTransform = CTransform::Create(m_pGraphic_Device);

	//Safe_AddRef(m_pTransform);

	/*if (m_pTransform == nullptr)
		return E_FAIL;*/

	if (pArg != nullptr) //�ƱԸ�Ʈ�� ���� �� ���⸦ �����µ�, �� ���� �ۿ� ���µ� �� ������ ������ �����İ�
	{
		memcpy(&m_StateDesc, pArg, sizeof(COLLRECTDESC));

		D3DXMatrixIdentity(&m_StateDesc.StateMatrix);

		m_iNumVertices = 8;
		m_iStride = sizeof(VTXCUBETEX);
		m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
		m_ePrimitiveType = D3DPT_TRIANGLELIST;
		m_iNumPrimitive = 12;

		/* �������� �Ҵ��ߴ�. */
		if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iStride, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, 0)))
			return E_FAIL;

		VTXCUBETEX*			pVertices = nullptr;

		m_pVB->Lock(0, /*m_iNumVertices * m_iStride*/0, (void**)&pVertices, 0);


		pVertices[0].vPosition = m_vPoint[0] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);// + InitPos;
		pVertices[0].vTexture = pVertices[0].vPosition;

		pVertices[1].vPosition = m_vPoint[1] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);// +InitPos;
		pVertices[1].vTexture = pVertices[1].vPosition;

		pVertices[2].vPosition = m_vPoint[2] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);// +InitPos;
		pVertices[2].vTexture = pVertices[2].vPosition;

		pVertices[3].vPosition = m_vPoint[3] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);// +InitPos;
		pVertices[3].vTexture = pVertices[3].vPosition;

		pVertices[4].vPosition = m_vPoint[4] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);// + InitPos;
		pVertices[4].vTexture = pVertices[4].vPosition;

		pVertices[5].vPosition = m_vPoint[5] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);// +InitPos;
		pVertices[5].vTexture = pVertices[5].vPosition;

		pVertices[6].vPosition = m_vPoint[6] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);// +InitPos;
		pVertices[6].vTexture = pVertices[6].vPosition;

		pVertices[7].vPosition = m_vPoint[7] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);// +InitPos;
		pVertices[7].vTexture = pVertices[7].vPosition;
		m_pVB->Unlock();

		m_iIndicesByte = sizeof(FACEINDICES16);
		m_eIndexFormat = D3DFMT_INDEX16;

		if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iNumPrimitive * m_iIndicesByte, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
			return E_FAIL;

		FACEINDICES16*	pIndices = nullptr;

		m_pIB->Lock(0, 0, (void**)&pIndices, 0);


		/* +X */
		pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
		pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

		/* -X */
		pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
		pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

		/* +Y */
		pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
		pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

		/* -Y */
		pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
		pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

		/* +Z */
		pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
		pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

		/* -Z */
		pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
		pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;


		m_pIB->Unlock();
	}


	return S_OK;
}

HRESULT CCollider_Cube::Update_ColliderBox(const _float4x4* WorldMatrix)
{
	m_StateDesc.StateMatrix = *WorldMatrix;

	m_vPoint[0] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[1] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[2] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[3] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[4] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);
	m_vPoint[5] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);
	m_vPoint[6] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);
	m_vPoint[7] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);

	_float3 vecOffsetPos = *(_float3*)&(m_StateDesc.StateMatrix.m[3][0]);
	vecOffsetPos.x += m_StateDesc.fOffSetX;
	vecOffsetPos.y += m_StateDesc.fOffSetY;
	vecOffsetPos.z += m_StateDesc.fOffsetZ;

	m_StateDesc.StateMatrix.m[3][0] = vecOffsetPos.x;
	m_StateDesc.StateMatrix.m[3][1] = vecOffsetPos.y;
	m_StateDesc.StateMatrix.m[3][2] = vecOffsetPos.z;

	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &m_StateDesc.StateMatrix);
	}
	
	Calculate_TransInfo();

	return S_OK;
}

HRESULT CCollider_Cube::Render_ColliderBox(_bool isHit)
{
	// ���� ��ȯ ��� ����
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_StateDesc.StateMatrix);

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

	return S_OK;
}

//#include <D3DX10math.h>  // D3DXVECTOR3 �� ���� �Լ� ����� ���� ����

_bool CCollider_Cube::Collision_Check(CCollider_Cube* pTarget, _float3* pOutDistance)
{
	if (pTarget == nullptr)
		return false;

	// �� OBB�� �߽�, �� ũ��, ȸ�� ��(����ȭ�� ����) ��������
	D3DXVECTOR3 centerA = this->m_vCenter;
	D3DXVECTOR3 halfA = this->m_vHalfExtents;
	D3DXVECTOR3 axesA[3] = { this->m_vAxisX, this->m_vAxisY, this->m_vAxisZ };

	D3DXVECTOR3 centerB = pTarget->m_vCenter;
	D3DXVECTOR3 halfB = pTarget->m_vHalfExtents;
	D3DXVECTOR3 axesB[3] = { pTarget->m_vAxisX, pTarget->m_vAxisY, pTarget->m_vAxisZ };

	// OBB�� 8�� ������ ��� (center + ȸ����� * ������)
	D3DXVECTOR3 cornersA[8];
	for (int i = 0; i < 8; ++i)
	{
		float offsetX = (i & 1) ? halfA.x : -halfA.x;
		float offsetY = (i & 2) ? halfA.y : -halfA.y;
		float offsetZ = (i & 4) ? halfA.z : -halfA.z;
		cornersA[i] = centerA + axesA[0] * offsetX +
			axesA[1] * offsetY +
			axesA[2] * offsetZ;
	}

	D3DXVECTOR3 cornersB[8];
	for (int i = 0; i < 8; ++i)
	{
		float offsetX = (i & 1) ? halfB.x : -halfB.x;
		float offsetY = (i & 2) ? halfB.y : -halfB.y;
		float offsetZ = (i & 4) ? halfB.z : -halfB.z;
		cornersB[i] = centerB + axesB[0] * offsetX +
			axesB[1] * offsetY +
			axesB[2] * offsetZ;
	}

	// SAT�� ���� �׽�Ʈ ����� �����մϴ�.
	// A�� B�� ���� �� 3������ �� ������ ���� �� ���� �� (�ִ� 9��)�� �����մϴ�.
	std::vector<D3DXVECTOR3> testAxes;
	// A�� �� �߰�
	testAxes.push_back(axesA[0]);
	testAxes.push_back(axesA[1]);
	testAxes.push_back(axesA[2]);
	// B�� �� �߰�
	testAxes.push_back(axesB[0]);
	testAxes.push_back(axesB[1]);
	testAxes.push_back(axesB[2]);
	// �� ������ ���� ������ �߰� (0�� ����� ������ ���� ����)
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			D3DXVECTOR3 axis;
			D3DXVec3Cross(&axis, &axesA[i], &axesB[j]);
			if (D3DXVec3Length(&axis) > 1e-6f)
			{
				D3DXVec3Normalize(&axis, &axis);
				testAxes.push_back(axis);
			}
		}
	}

	// �� �࿡ ���� �� ������ ���� �� ��ħ(������) ���ο� ħ�� ���̸� ����մϴ�.
	float minPenetration = FLT_MAX;
	D3DXVECTOR3 smallestAxis(0, 0, 0);
	for (size_t i = 0; i < testAxes.size(); ++i)
	{
		D3DXVECTOR3 axis = testAxes[i];

		// ���� A�� ���� ���� ���
		float minA = FLT_MAX, maxA = -FLT_MAX;
		for (int j = 0; j < 8; ++j)
		{
			float projection = D3DXVec3Dot(&cornersA[j], &axis);
			minA = min(minA, projection);
			maxA = max(maxA, projection);
		}
		// ���� B�� ���� ���� ���
		float minB = FLT_MAX, maxB = -FLT_MAX;
		for (int j = 0; j < 8; ++j)
		{
			float projection = D3DXVec3Dot(&cornersB[j], &axis);
			minB = min(minB, projection);
			maxB = max(maxB, projection);
		}

		// ���� ������ �и�(��ġ�� ����)�� ������ �浹���� ����
		if (maxA < minB || maxB < minA)
			return false;
		else
		{
			// ��ġ�� ��(������) ���
			float overlap = min(maxA, maxB) - max(minA, minB);
			if (overlap < minPenetration)
			{
				minPenetration = overlap;
				smallestAxis = axis;
			}
		}
	}

	// �浹�� �߻��� ���, �ɼ����� �浹 ����(�ּ� ħ�� ���̿� ����)�� ����մϴ�.
	if (pOutDistance)
	{
		// ���� A���� B�� ���� ����
		D3DXVECTOR3 d = centerB - centerA;
		// ���� d�� �ּ� ħ�� ���� ������ �ݴ��̸� ������ ������ŵ�ϴ�.
		if (D3DXVec3Dot(&d, &smallestAxis) < 0)
			smallestAxis = -smallestAxis;
		*pOutDistance = _float3(smallestAxis.x * minPenetration,
			smallestAxis.y * minPenetration,
			smallestAxis.z * minPenetration);
	}

	return true;
}





void CCollider_Cube::Calculate_TransInfo()
{
	// ���� ����� �����Ѵٰ� �����մϴ�.
    D3DXMATRIX matWorld = m_StateDesc.StateMatrix;  // ���� ��� (���� ��ü�� ��ȯ ����)

    // ���� ��Ŀ��� ��ġ ����
    m_vCenter = D3DXVECTOR3(matWorld._41, matWorld._42, matWorld._43);

    // ���� ����� ���� 3x3 �κп��� ȸ�� ����� �����Ͽ� �� ���� ���
    m_vAxisX = D3DXVECTOR3(matWorld._11, matWorld._12, matWorld._13);  // X��
    m_vAxisY = D3DXVECTOR3(matWorld._21, matWorld._22, matWorld._23);  // Y��
    m_vAxisZ = D3DXVECTOR3(matWorld._31, matWorld._32, matWorld._33);  // Z��

    // ���� ��Ŀ��� ũ�� (������) ���� (ȸ���� ����� ũ���̹Ƿ�, ���� ũ��)
 //   _float fScale;
	//fScale = D3DXVec3Length(&m_vAxisX); // X�� ������ ���� = ������ X
 //   m_vHalfExtents.x = fScale / 2.0f;  // ũ���� ������ ���

	//fScale = D3DXVec3Length(&m_vAxisY); // Y�� ������ ���� = ������ Y
 //   m_vHalfExtents.y = fScale / 2.0f;  // ũ���� ������ ���

	//fScale = D3DXVec3Length(&m_vAxisZ); // Z�� ������ ���� = ������ Z
 //   m_vHalfExtents.z = fScale / 2.0f;  // ũ���� ������ ���

	m_vHalfExtents.x = m_StateDesc.fRadiusX;
	m_vHalfExtents.y = m_StateDesc.fOffSetY;
	m_vHalfExtents.z = m_StateDesc.fOffsetZ;
}

CCollider_Cube * CCollider_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device/*, COLLRECTDESC& Des*/)
{
	CCollider_Cube*	pInstance = new CCollider_Cube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(/*Des*/)))
	{
		MSG_BOX("Failed to Created : CCollider_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_Cube::Clone(void * pArg)
{
	CCollider_Cube*	pInstance = new CCollider_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCollider_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Cube::Free()
{
	__super::Free();

	//Safe_Release(m_pTransform);
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
