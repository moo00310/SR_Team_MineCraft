#include "..\Public\Collider_Cube.h"


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

	// 상태 복구 - 텍스처 팩터 상태를 기본값으로 복구
	m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);

	// 텍스처 단계 복구
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	return S_OK;

}

_bool CCollider_Cube::Collision_Check(CCollider_Cube* pTarget, _Out_ _float3* pOutDistance, _Out_ COLLSION_DIR* pOutDir)
{
	if(pOutDistance)
		*pOutDistance = { 0.f, 0.f, 0.f };

	COLLSION_DIR Collision_Dir{ COLLSION_DIR::NONE };
	if (pOutDir)
		*pOutDir = Collision_Dir;

	if (pTarget == nullptr)
		return false;

	// 월드 행렬 추출
	const _float4x4* pWorldMatrixA = m_pTransformCom->Get_WorldMatrix();
	const _float4x4* pWorldMatrixB = pTarget->m_pTransformCom->Get_WorldMatrix();

	// 로컬 축 및 중심 계산
	_float3 axesA[3] = {
		_float3(pWorldMatrixA->_11, pWorldMatrixA->_12, pWorldMatrixA->_13),
		_float3(pWorldMatrixA->_21, pWorldMatrixA->_22, pWorldMatrixA->_23),
		_float3(pWorldMatrixA->_31, pWorldMatrixA->_32, pWorldMatrixA->_33)
	};
	_float3 centerA(pWorldMatrixA->_41, pWorldMatrixA->_42, pWorldMatrixA->_43);
	_float3 halfA = { m_StateDesc.fRadiusX, m_StateDesc.fRadiusY, m_StateDesc.fRadiusZ };

	_float3 axesB[3] = {
		_float3(pWorldMatrixB->_11, pWorldMatrixB->_12, pWorldMatrixB->_13),
		_float3(pWorldMatrixB->_21, pWorldMatrixB->_22, pWorldMatrixB->_23),
		_float3(pWorldMatrixB->_31, pWorldMatrixB->_32, pWorldMatrixB->_33)
	};
	_float3 centerB(pWorldMatrixB->_41, pWorldMatrixB->_42, pWorldMatrixB->_43);
	_float3 halfB = { pTarget->m_StateDesc.fRadiusX, pTarget->m_StateDesc.fRadiusY, pTarget->m_StateDesc.fRadiusZ };

	// 정규화
	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3Normalize(&axesA[i], &axesA[i]);
		D3DXVec3Normalize(&axesB[i], &axesB[i]);
	}

	// 8개 꼭짓점 계산
	_float3 cornersA[8], cornersB[8];
	for (int i = 0; i < 8; ++i)
	{
		float offsetX = (i & 1) ? halfA.x : -halfA.x;
		float offsetY = (i & 2) ? halfA.y : -halfA.y;
		float offsetZ = (i & 4) ? halfA.z : -halfA.z;
		cornersA[i] = centerA + axesA[0] * offsetX + axesA[1] * offsetY + axesA[2] * offsetZ;

		offsetX = (i & 1) ? halfB.x : -halfB.x;
		offsetY = (i & 2) ? halfB.y : -halfB.y;
		offsetZ = (i & 4) ? halfB.z : -halfB.z;
		cornersB[i] = centerB + axesB[0] * offsetX + axesB[1] * offsetY + axesB[2] * offsetZ;
	}

	// SAT 충돌 검사
	std::vector<_float3> testAxes = { axesA[0], axesA[1], axesA[2], axesB[0], axesB[1], axesB[2] };
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

	float minPenetration = FLT_MAX;
	_float3 smallestAxis(0, 0, 0);
	for (const auto& axis : testAxes)
	{
		float minA = FLT_MAX, maxA = -FLT_MAX;
		float minB = FLT_MAX, maxB = -FLT_MAX;

		for (int i = 0; i < 8; ++i)
		{
			float projA = D3DXVec3Dot(&cornersA[i], &axis);
			minA = min(minA, projA);
			maxA = max(maxA, projA);

			float projB = D3DXVec3Dot(&cornersB[i], &axis);
			minB = min(minB, projB);
			maxB = max(maxB, projB);
		}

		if (maxA < minB || maxB < minA)
			return false;

		float overlap = min(maxA, maxB) - max(minA, minB);
		if (overlap < minPenetration)
		{
			minPenetration = overlap;
			smallestAxis = axis;
		}
	}

	// 충돌 방향 분석 (위/아래/옆 구분)
	if (pOutDistance)
	{
		_float3 d = centerB - centerA;
		if (D3DXVec3Dot(&d, &smallestAxis) < 0)
			smallestAxis = -smallestAxis;

		// minPenetration이 정상적인 값인지 확인
		if (minPenetration <= 0)
			return false;

		*pOutDistance = smallestAxis * minPenetration;

		// pOutDistance 값이 이상하면 0 벡터로 보정
		if (D3DXVec3LengthSq(pOutDistance) < 1e-6f)
			*pOutDistance = { 0, 0, 0 };

		// 충돌 방향 판정
		if (fabs(smallestAxis.y) > fabs(smallestAxis.x) && fabs(smallestAxis.y) > fabs(smallestAxis.z))
		{
			if (smallestAxis.y > 0)
			{
				Collision_Dir = COLLSION_DIR::DOWN;
				//printf("바닥 충돌\n");
			}
			else
			{
				Collision_Dir = COLLSION_DIR::UP;
				//printf("천장 충돌\n");
			}
		}
		else if (fabs(smallestAxis.x) > fabs(smallestAxis.z))
		{
			if (smallestAxis.x > 0)
			{
				Collision_Dir = COLLSION_DIR::LEFT;
				//printf("왼쪽 충돌\n");
			}
			else
			{
				Collision_Dir = COLLSION_DIR::RIGHT;
				//printf("오른쪽 충돌\n");
			}
		}
		else
		{
			if (smallestAxis.z > 0)
			{
				Collision_Dir = COLLSION_DIR::FRONT;
				//printf("앞쪽 충돌\n");
			}
			else
			{
				Collision_Dir = COLLSION_DIR::BACK;
				//printf("뒤쪽 충돌\n");
			}
		}
	}

	if (pOutDir)
		*pOutDir = Collision_Dir;

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
