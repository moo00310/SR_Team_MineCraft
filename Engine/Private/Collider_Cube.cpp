#include "..\Public\Collider_Cube.h"


//const _tchar* CCollider_Cube::m_pTransformTag = TEXT("Com_Transform");

//_bool m_bRender = true;

CCollider_Cube::CCollider_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{

}

CCollider_Cube::CCollider_Cube(const CCollider_Cube& Prototype)
	: CComponent(Prototype)
	, m_pVB(Prototype.m_pVB)
	, m_pIB(Prototype.m_pIB)
	, m_iNumVertices(Prototype.m_iNumVertices)
	, m_dwFVF(Prototype.m_dwFVF)
	, m_iStride(Prototype.m_iStride)
	, m_ePrimitiveType(Prototype.m_ePrimitiveType)
	, m_iNumPrimitive(Prototype.m_iNumPrimitive)
{
	memcpy(m_vPoint, Prototype.m_vPoint, sizeof(_float3) * 4);
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CCollider_Cube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider_Cube::Initialize(void * pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	memcpy(&m_StateDesc, pArg, sizeof(COLLRECTDESC));
	m_pTransformCom = m_StateDesc.pTransformCom;

	if (!m_pTransformCom)
		return E_FAIL;
		
	Safe_AddRef(m_pTransformCom);

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

	return S_OK;
}

HRESULT CCollider_Cube::Update_ColliderBox()
{
	const _float4x4* pWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	_float4x4 StateMatrix = *pWorldMatrix;

	m_vPoint[0] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[1] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[2] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[3] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, -m_StateDesc.fRadiusZ);
	m_vPoint[4] = _float3(-m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);
	m_vPoint[5] = _float3(m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);
	m_vPoint[6] = _float3(m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);
	m_vPoint[7] = _float3(-m_StateDesc.fRadiusX, -m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ);

	_float3 vecOffsetPos = *(_float3*)&(StateMatrix.m[3][0]);
	vecOffsetPos.x += m_StateDesc.fOffSetX;
	vecOffsetPos.y += m_StateDesc.fOffSetY;
	vecOffsetPos.z += m_StateDesc.fOffsetZ;

	StateMatrix.m[3][0] = vecOffsetPos.x;
	StateMatrix.m[3][1] = vecOffsetPos.y;
	StateMatrix.m[3][2] = vecOffsetPos.z;

	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &StateMatrix);
	}

	return S_OK;
}

HRESULT CCollider_Cube::Render_ColliderBox(_bool isHit)
{
	// 세계 변환 행렬 설정
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

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

_bool CCollider_Cube::Collision_Check(CCollider_Cube* pTarget, _float3* pOutDistance)
{
	if (pTarget == nullptr)
		return false;

	// this 객체의 월드 매트릭스 추출
	const _float4x4* pWorldMatrixA = m_pTransformCom->Get_WorldMatrix();
	// 타겟 객체의 월드 매트릭스 추출
	const _float4x4* pWorldMatrixB = pTarget->m_pTransformCom->Get_WorldMatrix();

	// 월드 매트릭스에서 로컬 축과 중심 추출 (D3DXMATRIX 스타일 가정: _11~_33은 회전, _41~_43은 번역)
	_float3 axesA[3] = {
		_float3(pWorldMatrixA->_11, pWorldMatrixA->_12, pWorldMatrixA->_13),
		_float3(pWorldMatrixA->_21, pWorldMatrixA->_22, pWorldMatrixA->_23),
		_float3(pWorldMatrixA->_31, pWorldMatrixA->_32, pWorldMatrixA->_33)
	};
	_float3 centerA(pWorldMatrixA->_41, pWorldMatrixA->_42, pWorldMatrixA->_43);
	// 정규화
	D3DXVec3Normalize(&axesA[0], &axesA[0]);
	D3DXVec3Normalize(&axesA[1], &axesA[1]);
	D3DXVec3Normalize(&axesA[2], &axesA[2]);

	// this 객체의 반 크기 (로컬 공간에서 정의된 값)
	_float3 halfA = { m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ };

	// this 객체의 8개 꼭짓점 계산
	_float3 cornersA[8];
	for (int i = 0; i < 8; ++i)
	{
		float offsetX = (i & 1) ? halfA.x : -halfA.x;
		float offsetY = (i & 2) ? halfA.y : -halfA.y;
		float offsetZ = (i & 4) ? halfA.z : -halfA.z;
		cornersA[i] = centerA + axesA[0] * offsetX +
			axesA[1] * offsetY +
			axesA[2] * offsetZ;
	}

	// 타겟 객체의 월드 매트릭스에서 로컬 축과 중심 추출
	_float3 axesB[3] = {
		_float3(pWorldMatrixB->_11, pWorldMatrixB->_12, pWorldMatrixB->_13),
		_float3(pWorldMatrixB->_21, pWorldMatrixB->_22, pWorldMatrixB->_23),
		_float3(pWorldMatrixB->_31, pWorldMatrixB->_32, pWorldMatrixB->_33)
	};
	_float3 centerB(pWorldMatrixB->_41, pWorldMatrixB->_42, pWorldMatrixB->_43);
	D3DXVec3Normalize(&axesB[0], &axesB[0]);
	D3DXVec3Normalize(&axesB[1], &axesB[1]);
	D3DXVec3Normalize(&axesB[2], &axesB[2]);

	// 타겟 객체의 반 크기
	_float3 halfB = { pTarget->m_StateDesc.fRadiusX,pTarget->m_StateDesc.fRadiusY, pTarget->m_StateDesc.fRadiusZ };

	// 타겟 객체의 8개 꼭짓점 계산
	_float3 cornersB[8];
	for (int i = 0; i < 8; ++i)
	{
		float offsetX = (i & 1) ? halfB.x : -halfB.x;
		float offsetY = (i & 2) ? halfB.y : -halfB.y;
		float offsetZ = (i & 4) ? halfB.z : -halfB.z;
		cornersB[i] = centerB + axesB[0] * offsetX +
			axesB[1] * offsetY +
			axesB[2] * offsetZ;
	}

	// SAT(Separating Axis Theorem)를 위한 테스트 축 설정
	std::vector<_float3> testAxes;
	// 각 객체의 로컬 축 3개씩
	testAxes.push_back(axesA[0]);
	testAxes.push_back(axesA[1]);
	testAxes.push_back(axesA[2]);
	testAxes.push_back(axesB[0]);
	testAxes.push_back(axesB[1]);
	testAxes.push_back(axesB[2]);
	// 두 객체의 축 교차 결과 (길이가 매우 짧은 축은 제외)
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			_float3 axis;
			D3DXVec3Cross(&axis, &axesA[i], &axesB[j]);
			if (D3DXVec3Length(&axis) > 1e-6f)
			{
				D3DXVec3Normalize(&axis, &axis);
				testAxes.push_back(axis);
			}
		}
	}

	// 각 축에 대해 투영 구간을 계산하고, 겹치는지 검사합니다.
	float minPenetration = FLT_MAX;
	_float3 smallestAxis(0, 0, 0);
	for (const auto& axis : testAxes)
	{
		// this 객체 투영 구간
		float minA = FLT_MAX, maxA = -FLT_MAX;
		for (int i = 0; i < 8; ++i)
		{
			float proj = D3DXVec3Dot(&cornersA[i], &axis);
			minA = min(minA, proj);
			maxA = max(maxA, proj);
		}
		// 타겟 객체 투영 구간
		float minB = FLT_MAX, maxB = -FLT_MAX;
		for (int i = 0; i < 8; ++i)
		{
			float proj = D3DXVec3Dot(&cornersB[i], &axis);
			minB = min(minB, proj);
			maxB = max(maxB, proj);
		}

		// 투영 구간에 분리(갭)가 있다면 충돌 없음
		if (maxA < minB || maxB < minA)
			return false;
		else
		{
			// 겹치는 정도(오버랩)를 계산
			float overlap = min(maxA, maxB) - max(minA, minB);
			if (overlap < minPenetration)
			{
				minPenetration = overlap;
				smallestAxis = axis;
			}
		}
	}

	// 충돌이 발생한 경우, 옵션으로 최소 침투 깊이와 방향을 계산
	if (pOutDistance)
	{
		// 두 중심 간 벡터
		_float3 d = centerB - centerA;
		// 두 벡터의 방향이 반대면 최소 축의 방향을 반전
		if (D3DXVec3Dot(&d, &smallestAxis) < 0)
			smallestAxis = -smallestAxis;
		*pOutDistance = _float3(smallestAxis.x * minPenetration,
			smallestAxis.y * minPenetration,
			smallestAxis.z * minPenetration);
	}

	return true;
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

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
