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

	///* 정점들을 할당했다. */
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

	if (pArg != nullptr) //아규먼트가 있을 때 여기를 들어오는데, 이 차이 밖에 없는데 왜 릴리즈 에러가 터지냐고
	{
		memcpy(&m_StateDesc, pArg, sizeof(COLLRECTDESC));

		D3DXMatrixIdentity(&m_StateDesc.StateMatrix);

		m_iNumVertices = 8;
		m_iStride = sizeof(VTXCUBETEX);
		m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
		m_ePrimitiveType = D3DPT_TRIANGLELIST;
		m_iNumPrimitive = 12;

		/* 정점들을 할당했다. */
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
	

	return S_OK;
}

HRESULT CCollider_Cube::Render_ColliderBox(_bool isHit)
{
	// 세계 변환 행렬 설정
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_StateDesc.StateMatrix);

	// 와이어프레임 모드 설정
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// bRed가 true일 때 빨간색 적용
	if (isHit)
	{
		m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 0, 0)); // 빨간색
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	}
	else
	{
		// 원래 텍스처를 사용하도록 설정 복구
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}

	// 정점 및 인덱스 버퍼 설정
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphic_Device->SetFVF(m_dwFVF);
	m_pGraphic_Device->SetIndices(m_pIB);

	// 프리미티브(도형) 그리기
	m_pGraphic_Device->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	// 원래 상태(솔리드 모드)로 복구
	m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}

//#include <D3DX10math.h>  // D3DXVECTOR3 및 관련 함수 사용을 위해 포함

_bool CCollider_Cube::Collision_Check(CCollider_Cube* pTarget, _float3* pOutDistance)
{
	if (!pTarget) return false;

	// 각 OBB의 8개의 꼭짓점 위치를 계산합니다.
	D3DXVECTOR3 vA[8], vB[8];

	// 자신의 OBB (this)
	D3DXVECTOR3 vCenterA(this->m_vCenter);  // OBB의 중심
	D3DXVECTOR3 vHalfExtentsA(this->m_vHalfExtents);  // OBB의 반 크기 (반 길이)

	// 타겟 OBB (pTarget)
	D3DXVECTOR3 vCenterB(pTarget->m_vCenter);
	D3DXVECTOR3 vHalfExtentsB(pTarget->m_vHalfExtents);

	// OBB의 각 축 (회전, 크기 정보 등) 계산을 위해 방향 벡터가 필요합니다.
	D3DXVECTOR3 vAxesA[3] = { this->m_vAxisX, this->m_vAxisY, this->m_vAxisZ };
	D3DXVECTOR3 vAxesB[3] = { pTarget->m_vAxisX, pTarget->m_vAxisY, pTarget->m_vAxisZ };

	// OBB의 8개의 꼭짓점 좌표 계산
	for (int i = 0; i < 8; ++i)
	{
		vA[i] = vCenterA;

		if (i & 1) vA[i].x += vHalfExtentsA.x;
		else vA[i].x -= vHalfExtentsA.x;

		if (i & 2) vA[i].y += vHalfExtentsA.y;
		else vA[i].y -= vHalfExtentsA.y;

		if (i & 4) vA[i].z += vHalfExtentsA.z;
		else vA[i].z -= vHalfExtentsA.z;

		// 회전 적용
		vA[i] = D3DXVECTOR3(
			vAxesA[0].x * vA[i].x + vAxesA[1].x * vA[i].y + vAxesA[2].x * vA[i].z,
			vAxesA[0].y * vA[i].x + vAxesA[1].y * vA[i].y + vAxesA[2].y * vA[i].z,
			vAxesA[0].z * vA[i].x + vAxesA[1].z * vA[i].y + vAxesA[2].z * vA[i].z
		);
	}

	// 타겟 OBB의 8개의 꼭짓점 좌표 계산
	for (int i = 0; i < 8; ++i)
	{
		vB[i] = vCenterB;

		if (i & 1) vB[i].x += vHalfExtentsB.x;
		else vB[i].x -= vHalfExtentsB.x;

		if (i & 2) vB[i].y += vHalfExtentsB.y;
		else vB[i].y -= vHalfExtentsB.y;

		if (i & 4) vB[i].z += vHalfExtentsB.z;
		else vB[i].z -= vHalfExtentsB.z;

		// 회전 적용
		vB[i] = D3DXVECTOR3(
			vAxesB[0].x * vB[i].x + vAxesB[1].x * vB[i].y + vAxesB[2].x * vB[i].z,
			vAxesB[0].y * vB[i].x + vAxesB[1].y * vB[i].y + vAxesB[2].y * vB[i].z,
			vAxesB[0].z * vB[i].x + vAxesB[1].z * vB[i].y + vAxesB[2].z * vB[i].z
		);
	}

	// OBB의 축에 대해 투영을 수행하고, 두 OBB가 겹치는지 체크합니다.
	D3DXVECTOR3 vAxis[6] = {
		vAxesA[0], vAxesA[1], vAxesA[2],  // A OBB의 축
		vAxesB[0], vAxesB[1], vAxesB[2]   // B OBB의 축
	};

	for (int i = 0; i < 6; ++i)
	{
		// 각 축에 대해 두 OBB를 투영시킵니다.
		float minA = FLT_MAX, maxA = -FLT_MAX;
		float minB = FLT_MAX, maxB = -FLT_MAX;

		for (int j = 0; j < 8; ++j)
		{
			float projectionA = D3DXVec3Dot(&vA[j], &vAxis[i]);
			minA = min(minA, projectionA);
			maxA = max(maxA, projectionA);

			float projectionB = D3DXVec3Dot(&vB[j], &vAxis[i]);
			minB = min(minB, projectionB);
			maxB = max(maxB, projectionB);
		}

		// 투영이 겹치지 않으면 충돌이 없다고 판단합니다.
		if (maxA < minB || maxB < minA)
		{
			return false;  // 충돌 없음
		}
	}

	// OBB 충돌이 발생한 경우
	return true;
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
