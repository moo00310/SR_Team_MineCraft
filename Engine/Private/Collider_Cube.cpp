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

	COLLCUBE_DESC tDesc{};

	memcpy(&tDesc, pArg, sizeof(COLLCUBE_DESC));
	m_pTransformCom = tDesc.pTransformCom;
	m_pOwner = tDesc.pOwner;

	m_vRadius = tDesc.vRadius;
	m_vOffset = tDesc.vOffset;

	if (!m_pTransformCom)
		return E_FAIL;

	if (!m_pOwner)
		return E_FAIL;

	Safe_AddRef(m_pTransformCom);

	m_iNumVertices = 8;
	m_iStride = sizeof(VTXCUBETEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 12;

	/* 정점 버퍼 생성 */
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iStride, 0, m_dwFVF, D3DPOOL_MANAGED, &m_pVB, 0)))
		return E_FAIL;

	VTXCUBETEX* pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = m_vPoint[0] = _float3(-m_vRadius.x, m_vRadius.y, -m_vRadius.z) + m_vOffset;
	pVertices[1].vPosition = m_vPoint[1] = _float3(m_vRadius.x, m_vRadius.y, -m_vRadius.z) + m_vOffset;
	pVertices[2].vPosition = m_vPoint[2] = _float3(m_vRadius.x, -m_vRadius.y, -m_vRadius.z) + m_vOffset;
	pVertices[3].vPosition = m_vPoint[3] = _float3(-m_vRadius.x, -m_vRadius.y, -m_vRadius.z) + m_vOffset;
	pVertices[4].vPosition = m_vPoint[4] = _float3(-m_vRadius.x, m_vRadius.y, m_vRadius.z) + m_vOffset;
	pVertices[5].vPosition = m_vPoint[5] = _float3(m_vRadius.x, m_vRadius.y, m_vRadius.z) + m_vOffset;
	pVertices[6].vPosition = m_vPoint[6] = _float3(m_vRadius.x, -m_vRadius.y, m_vRadius.z) + m_vOffset;
	pVertices[7].vPosition = m_vPoint[7] = _float3(-m_vRadius.x, -m_vRadius.y, m_vRadius.z) + m_vOffset;


	for (int i = 0; i < 8; i++)
		pVertices[i].vTexture = pVertices[i].vPosition;

	m_pVB->Unlock();

	m_iIndicesByte = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	/* 인덱스 버퍼 생성 */
	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iNumPrimitive * m_iIndicesByte, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	/* 인덱스 설정 */
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

	// 기본 8개의 꼭짓점 설정
	m_vPoint[0] = _float3(-m_vRadius.x, m_vRadius.y, -m_vRadius.z);
	m_vPoint[1] = _float3(m_vRadius.x, m_vRadius.y, -m_vRadius.z);
	m_vPoint[2] = _float3(m_vRadius.x, -m_vRadius.y, -m_vRadius.z);
	m_vPoint[3] = _float3(-m_vRadius.x, -m_vRadius.y, -m_vRadius.z);
	m_vPoint[4] = _float3(-m_vRadius.x, m_vRadius.y, m_vRadius.z);
	m_vPoint[5] = _float3(m_vRadius.x, m_vRadius.y, m_vRadius.z);
	m_vPoint[6] = _float3(m_vRadius.x, -m_vRadius.y, m_vRadius.z);
	m_vPoint[7] = _float3(-m_vRadius.x, -m_vRadius.y, m_vRadius.z);


	// 월드 위치에 offset 적용
	_float3 vecOffsetPos = *(_float3*)&(StateMatrix.m[3][0]);

	vecOffsetPos += m_vOffset;

	// 월드 행렬의 위치 부분 업데이트
	StateMatrix.m[3][0] = vecOffsetPos.x;
	StateMatrix.m[3][1] = vecOffsetPos.y;
	StateMatrix.m[3][2] = vecOffsetPos.z;

	// 8개의 꼭짓점을 월드 행렬로 변환
	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &StateMatrix);
	}

	return S_OK;
}


HRESULT CCollider_Cube::Render_Collider(_bool isHit)
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

_bool CCollider_Cube::Collision_Check(CCollider_Cube* pTarget, _Out_ _float3* pOutDistance, _Out_ CCollider::COLLISION_DIR* pOutDir)
{
	if (pOutDistance)
		*pOutDistance = { 0.f, 0.f, 0.f };

	COLLISION_DIR Collision_Dir{ COLLISION_DIR::NONE };
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

	// offset 적용된 중심 계산
	_float3 centerA(pWorldMatrixA->_41 + m_vOffset.x,
		pWorldMatrixA->_42 + m_vOffset.y,
		pWorldMatrixA->_43 + m_vOffset.z);

	_float3 halfA = { m_vRadius.x, m_vRadius.y, m_vRadius.z };

	_float3 axesB[3] = {
		_float3(pWorldMatrixB->_11, pWorldMatrixB->_12, pWorldMatrixB->_13),
		_float3(pWorldMatrixB->_21, pWorldMatrixB->_22, pWorldMatrixB->_23),
		_float3(pWorldMatrixB->_31, pWorldMatrixB->_32, pWorldMatrixB->_33)
	};

	// offset 적용된 중심 계산
	_float3 centerB(pWorldMatrixB->_41 + pTarget->m_vOffset.x,
		pWorldMatrixB->_42 + pTarget->m_vOffset.y,
		pWorldMatrixB->_43 + pTarget->m_vOffset.z);

	_float3 halfB = { pTarget->m_vRadius.x, pTarget->m_vRadius.y, pTarget->m_vRadius.z };

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
				Collision_Dir = COLLISION_DIR::DOWN;
			}
			else
			{
				Collision_Dir = COLLISION_DIR::UP;
			}
		}
		else if (fabs(smallestAxis.x) > fabs(smallestAxis.z))
		{
			if (smallestAxis.x > 0)
			{
				Collision_Dir = COLLISION_DIR::LEFT;
			}
			else
			{
				Collision_Dir = COLLISION_DIR::RIGHT;
			}
		}
		else
		{
			if (smallestAxis.z > 0)
			{
				Collision_Dir = COLLISION_DIR::FRONT;
			}
			else
			{
				Collision_Dir = COLLISION_DIR::BACK;
			}
		}
	}

	if (pOutDir)
		*pOutDir = Collision_Dir;

	return true;

	//if (pOutDistance)
	//	*pOutDistance = { 0.f, 0.f, 0.f };

	//CCollider_Cube::COLLISION_DIR Collision_Dir{ CCollider_Cube::COLLISION_DIR::NONE };
	//if (pOutDir)
	//	*pOutDir = Collision_Dir;

	//if (pTarget == nullptr)
	//	return false;

	//_float3 vMyPosition{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
	//_float3 vTargetPosition{ pTarget->m_pTransformCom->Get_State(CTransform::STATE_POSITION) };

	//// AABB 최소/최대 좌표 계산
	//_float3 minA = {vMyPosition.x - m_StateDesc.fRadiusX,
	//				vMyPosition.y - m_StateDesc.fRadiusY,
	//				vMyPosition.z - m_StateDesc.fRadiusZ };

	//_float3 maxA = {vMyPosition.x + m_StateDesc.fRadiusX,
	//				vMyPosition.y + m_StateDesc.fRadiusY,
	//				vMyPosition.z + m_StateDesc.fRadiusZ };

	//_float3 minB = {vTargetPosition.x - pTarget->m_StateDesc.fRadiusX,
	//				vTargetPosition.y - pTarget->m_StateDesc.fRadiusY,
	//				vTargetPosition.z - pTarget->m_StateDesc.fRadiusZ };

	//_float3 maxB = {vTargetPosition.x + pTarget->m_StateDesc.fRadiusX,
	//				vTargetPosition.y + pTarget->m_StateDesc.fRadiusY,
	//				vTargetPosition.z + pTarget->m_StateDesc.fRadiusZ };

	//// AABB 충돌 검사
	//if (maxA.x < minB.x || minA.x > maxB.x ||
	//	maxA.y < minB.y || minA.y > maxB.y ||
	//	maxA.z < minB.z || minA.z > maxB.z)
	//{
	//	return false;
	//}

	//// 충돌 방향 계산
	//_float3 overlap = { min(maxA.x, maxB.x) - max(minA.x, minB.x),
	//					min(maxA.y, maxB.y) - max(minA.y, minB.y),
	//					min(maxA.z, maxB.z) - max(minA.z, minB.z) };

	//if (pOutDistance)
	//	*pOutDistance = overlap;

	//if (pOutDir)
	//{
	//	if (overlap.y <= overlap.x && overlap.y <= overlap.z)
	//		Collision_Dir = (minA.y < minB.y) ? COLLISION_DIR::UP : COLLISION_DIR::DOWN;
	//	else if (overlap.x <= overlap.z)
	//		Collision_Dir = (minA.x < minB.x) ? COLLISION_DIR::RIGHT : COLLISION_DIR::LEFT;
	//	else
	//		Collision_Dir = (minA.z < minB.z) ? COLLISION_DIR::FRONT : COLLISION_DIR::BACK;

	//	*pOutDir = Collision_Dir;
	//}

	//return true;
}


// CCollider_Cube 클래스에 min, max 값을 리턴하는 함수 추가
// AABB의 최소값 반환
_float3 CCollider_Cube::GetMin() const
{
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// 최소 좌표 계산
	return vPosition - m_vRadius;
}

// AABB의 최대값 반환
_float3 CCollider_Cube::GetMax() const
{
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// 최대 좌표 계산
	return vPosition + m_vRadius;
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
