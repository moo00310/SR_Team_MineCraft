#include "Transform.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Collider.h"

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent { pGraphic_Device }
{
}

CTransform::CTransform(const CTransform& Prototype)
	: CComponent( Prototype )
	, m_WorldMatrix { Prototype.m_WorldMatrix }
{
}

HRESULT CTransform::Initialize_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);
		m_fSpeedPerSec = pDesc->fSpeedPerSec;
		m_fRotationPerSec = pDesc->fRotationPerSec;
	}

	return S_OK;
}

HRESULT CTransform::Bind_Resource()
{
	return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);	
}

HRESULT CTransform::Bind_Resource(CShader* pShader)
{
	D3DXMATRIX viewMatrix, projMatrix;
	// 디바이스에서 뷰 행렬 가져오기
	if (FAILED(m_pGraphic_Device->GetTransform(D3DTS_VIEW, &viewMatrix)))
		return E_FAIL;

	// 디바이스에서 투영 행렬 가져오기
	if (FAILED(m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &projMatrix)))
		return E_FAIL;

	if (FAILED(pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(pShader->Bind_Matrix("g_ViewMatrix", &viewMatrix)))
		return E_FAIL;
	if (FAILED(pShader->Bind_Matrix("g_ProjMatrix", &projMatrix)))
		return E_FAIL;

	return S_OK;
}

_float3 CTransform::Compute_Scaled() const
{
	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	return _float3(D3DXVec3Length(&vRight),
		D3DXVec3Length(&vUp),
		D3DXVec3Length(&vLook));
}

void CTransform::Scaling(_float fX, _float fY, _float fZ)
{
	_float3		vRight = Get_State(STATE_RIGHT);
	_float3		vUp = Get_State(STATE_UP);
	_float3		vLook = Get_State(STATE_LOOK);

	Set_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * fX);
	Set_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * fY);
	Set_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * fZ);
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;


	////콜라이더 정보를 던져주 는함수 따로 만들어야지
	//////여기서 충돌체크 하고 충돌이면 이동 안하게 하면 됨
	//m_pGameInstance->Collision_Check_Group_Multi()

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);

	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vRight = Get_State(STATE_RIGHT);

	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);

	vPosition += *D3DXVec3Normalize(&vUp, &vUp) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_float fTimeDelta)
{
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vUp = Get_State(STATE_UP);

	vPosition -= *D3DXVec3Normalize(&vUp, &vUp) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Direction(const _float3& _direction, _float fTimeDelta)
{
	_float3 vOriginPos{ Get_State(STATE_POSITION) };
	_float3 vNextPosition{ vOriginPos };
	_float3 vDirection{ _direction };

	vNextPosition += *D3DXVec3Normalize(&vDirection, &vDirection) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vNextPosition);
}

void CTransform::Go_Direction(CCollider* pCollider, _uint iGroupIndex, const _float3& vDirection, _float fTimeDelta)
{
	_float3 vOriginPos{ Get_State(STATE_POSITION) };
	_float3 vNextPosition{ vOriginPos };
	_float3 _vDirection{ vDirection };

	vNextPosition += *D3DXVec3Normalize(&_vDirection, &_vDirection) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vNextPosition);

	// 충돌 검사
	list<CCollider_Cube::COLLISION_INFO> CollisionObjects;
	m_pGameInstance->Collision_Check_Group_Multi(iGroupIndex, CollisionObjects, pCollider, CCollider_Manager::COLLSIION_CUBE);

	for (CCollider_Cube::COLLISION_INFO& tInfo : CollisionObjects)
	{
		//if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::FRONT ||
		//	tInfo.eCollisionDir == CCollider::COLLISION_DIR::BACK ||
		//	tInfo.eCollisionDir == CCollider::COLLISION_DIR::LEFT ||
		//	tInfo.eCollisionDir == CCollider::COLLISION_DIR::RIGHT)
		//{
		//	Set_State(STATE_POSITION, vOriginPos);
		//	return;
		//}

		if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::FRONT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::BACK)
		{
			vNextPosition.z = vOriginPos.z;
		}
		else if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::LEFT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::RIGHT)
		{
			vNextPosition.x = vOriginPos.x;
		}
	}

	Set_State(STATE_POSITION, vNextPosition);
}

void CTransform::Go_Straight(CCollider* pCollider, _uint iGroupIndex, _float fTimeDelta)
{
	_float3		vOriginPosition = Get_State(STATE_POSITION);
	_float3		vLook = Get_State(STATE_LOOK);

	_float3		vNextPosition{ vOriginPosition };
	vNextPosition += *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vNextPosition);

	list<CCollider_Cube::COLLISION_INFO> CollisionObjects;
	m_pGameInstance->Collision_Check_Group_Multi(iGroupIndex, CollisionObjects, pCollider, CCollider_Manager::COLLSIION_CUBE);

	for (CCollider_Cube::COLLISION_INFO& tInfo : CollisionObjects)
	{
		//1번 방법
		if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::FRONT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::BACK ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::LEFT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::RIGHT)
		{
			Set_State(STATE_POSITION, vOriginPosition);
			return;
		}

		//2번방법
		//이부분 벽에 들어가지느 버그 있음 근데 그냥 리턴하면 벽에 붙음 나중에 보기
		/*if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::FRONT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::BACK)
		{
			vNextPosition.z = vOriginPosition.z;
			Set_State(STATE_POSITION, vNextPosition);
		}
		else if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::LEFT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::RIGHT)
		{
			vNextPosition.x = vOriginPosition.x;
			Set_State(STATE_POSITION, vNextPosition);
		}*/

		//3 번방법
		/*if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::FRONT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::BACK)
		{
			vNextPosition.z = vOriginPosition.z;
		}
		else if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::LEFT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::RIGHT)
		{
			vNextPosition.x = vOriginPosition.x;
		}*/

	}
	//Set_State(STATE_POSITION, vNextPosition);
}

void CTransform::Go_Backward(CCollider* pCollider, _uint iGroupIndex, _float fTimeDelta)
{
	_float3 vOriginPosition = Get_State(STATE_POSITION);
	_float3 vLook = Get_State(STATE_LOOK);

	_float3 vNextPosition = vOriginPosition - *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeedPerSec * fTimeDelta;
	Set_State(STATE_POSITION, vNextPosition);

	list<CCollider_Cube::COLLISION_INFO> CollisionObjects;
	m_pGameInstance->Collision_Check_Group_Multi(iGroupIndex, CollisionObjects, pCollider, CCollider_Manager::COLLSIION_CUBE);

	for (CCollider_Cube::COLLISION_INFO& tInfo : CollisionObjects)
	{
		//1번 방법
		if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::FRONT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::BACK ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::LEFT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::RIGHT)
		{
			Set_State(STATE_POSITION, vOriginPosition);
			return;
		}
	}
}

void CTransform::Go_Left(CCollider* pCollider, _uint iGroupIndex, _float fTimeDelta)
{
	_float3 vOriginPosition = Get_State(STATE_POSITION);
	_float3 vRight = Get_State(STATE_RIGHT);

	_float3 vNextPosition = vOriginPosition - *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;
	Set_State(STATE_POSITION, vNextPosition);

	list<CCollider_Cube::COLLISION_INFO> CollisionObjects;
	m_pGameInstance->Collision_Check_Group_Multi(iGroupIndex, CollisionObjects, pCollider, CCollider_Manager::COLLSIION_CUBE);

	for (CCollider_Cube::COLLISION_INFO& tInfo : CollisionObjects)
	{
		//1번 방법
		if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::FRONT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::BACK ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::LEFT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::RIGHT)
		{
			Set_State(STATE_POSITION, vOriginPosition);
			return;
		}
	}

}

void CTransform::Go_Right(CCollider* pCollider, _uint iGroupIndex, _float fTimeDelta)
{
	_float3 vOriginPosition = Get_State(STATE_POSITION);
	_float3 vRight = Get_State(STATE_RIGHT);

	_float3 vNextPosition = vOriginPosition + *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeedPerSec * fTimeDelta;
	Set_State(STATE_POSITION, vNextPosition);

	list<CCollider_Cube::COLLISION_INFO> CollisionObjects;
	m_pGameInstance->Collision_Check_Group_Multi(iGroupIndex, CollisionObjects, pCollider, CCollider_Manager::COLLSIION_CUBE);

	for (CCollider_Cube::COLLISION_INFO& tInfo : CollisionObjects)
	{
		//1번 방법
		if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::FRONT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::BACK ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::LEFT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::RIGHT)
		{
			Set_State(STATE_POSITION, vOriginPosition);
			return;
		}
	}

}

void CTransform::Chase(CCollider* pCollider, _uint iGroupIndex, const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance)
{
	_float3 vOriginPos{ Get_State(STATE_POSITION) };
	_float3 vNextPosition{ vOriginPos };
	_float3 vMoveDir = vTargetPos - vNextPosition;

	if (fMinDistance <= D3DXVec3Length(&vMoveDir))
	{
		vNextPosition += *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * m_fSpeedPerSec * fTimeDelta;
	}

	Set_State(STATE_POSITION, vNextPosition);

	// 충돌 검사
	list<CCollider_Cube::COLLISION_INFO> CollisionObjects;
	m_pGameInstance->Collision_Check_Group_Multi(iGroupIndex, CollisionObjects, pCollider, CCollider_Manager::COLLSIION_CUBE);

	for (CCollider_Cube::COLLISION_INFO& tInfo : CollisionObjects)
	{
		if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::FRONT	||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::BACK	||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::LEFT	||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::RIGHT)
		{
			Set_State(STATE_POSITION, vOriginPos);
			return;
		}

		/*if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::FRONT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::BACK)
		{
			vNextPosition.z = vOriginPos.z;
		}
		else if (tInfo.eCollisionDir == CCollider::COLLISION_DIR::LEFT ||
			tInfo.eCollisionDir == CCollider::COLLISION_DIR::RIGHT)
		{
			vNextPosition.x = vOriginPos.x;
		}*/
	}

	//Set_State(STATE_POSITION, vNextPosition);
}



void CTransform::LookAt(const _float3& vTargetPos)
{
	_float3		vScaled = Compute_Scaled();
	_float3		vPosition = Get_State(STATE_POSITION);
	_float3		vLook = vTargetPos - vPosition;
	_float3		vRight = {};

	_float3		vUpDir{ 0.f, 1.f, 0.f };	
	D3DXVec3Cross(&vRight, &vUpDir, &vLook);

	_float3		vUp = {};
	D3DXVec3Cross(&vUp, &vLook, &vRight);

	Set_State(STATE_RIGHT, *D3DXVec3Normalize(&vRight, &vRight) * vScaled.x);
	Set_State(STATE_UP, *D3DXVec3Normalize(&vUp, &vUp) * vScaled.y);
	Set_State(STATE_LOOK, *D3DXVec3Normalize(&vLook, &vLook) * vScaled.z);
}

void CTransform::LookAt_XZ(const _float3& vTargetPos)
{
	_float3 vScaled = Compute_Scaled();
	_float3 vPosition = Get_State(STATE_POSITION);

	_float3 vLook = vTargetPos - vPosition;
	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);

	_float3 vWorldUp = { 0.f, 1.f, 0.f };

	// RIGHT는 수평 기준
	_float3 vRight;
	D3DXVec3Cross(&vRight, &vWorldUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	// 새롭게 계산된 UP (정확한 직교 축)
	_float3 vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	Set_State(STATE_RIGHT, vRight * vScaled.x);
	Set_State(STATE_UP, vUp * vScaled.y); // 여기 중요
	Set_State(STATE_LOOK, vLook * vScaled.z);
}

void CTransform::Chase(const _float3& vTargetPos, _float fTimeDelta, _float fMinDistance)
{
	_float3		vPosition = Get_State(STATE_POSITION);

	_float3		vMoveDir = vTargetPos - vPosition;

	if (fMinDistance <= D3DXVec3Length(&vMoveDir))
	{
		vPosition += *D3DXVec3Normalize(&vMoveDir, &vMoveDir) * m_fSpeedPerSec * fTimeDelta;
	}

	Set_State(STATE_POSITION, vPosition);
}


void CTransform::Turn(const _float3& vAxis, _float fTimeDelta)
{
	_float3			vRight = Get_State(STATE_RIGHT);
	_float3			vUp = Get_State(STATE_UP);
	_float3			vLook = Get_State(STATE_LOOK);

	_float4x4		RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, m_fRotationPerSec * fTimeDelta);
	
	// D3DXVec4Transform();
	// D3DXVec3TransformCoord();
	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotation(const _float3& vAxis, _float fRadian)
{
	_float3		vScaled = Compute_Scaled();

	_float3			vRight = _float3(1.f, 0.f, 0.f) * vScaled.x;
	_float3			vUp = _float3(0.f, 1.f, 0.f) * vScaled.y;
	_float3			vLook = _float3(0.f, 0.f, 1.f) * vScaled.z;

	_float4x4		RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	// D3DXVec4Transform();
	// D3DXVec3TransformCoord();
	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Quaternion_Turn(const _float3& vAngle)
{
	_float3            vRight = Get_State(STATE_RIGHT);
	_float3            vUp = Get_State(STATE_UP);
	_float3            vLook = Get_State(STATE_LOOK);

	// 사원수 회전 생성
	D3DXQUATERNION Qur{};
	D3DXQuaternionRotationYawPitchRoll(&Qur, vAngle.y, vAngle.x, vAngle.z);

	// 최종 벡터 상태 저장
	Set_State(STATE_RIGHT, RotateVectorByQuaternion(vRight, Qur));
	Set_State(STATE_UP, RotateVectorByQuaternion(vUp, Qur));
	Set_State(STATE_LOOK, RotateVectorByQuaternion(vLook, Qur));
}


void CTransform::Quaternion_Rotation(const _float3& vAngle)
{
	_float3 vScaled = Compute_Scaled();

	_float3 vRight = _float3{ 1.f, 0.f, 0.f } *vScaled.x;
	_float3 vUp = _float3{ 0.f, 1.f, 0.f } *vScaled.y;
	_float3 vLook = _float3{ 0.f, 0.f, 1.f } *vScaled.z;

	// 사원수 회전 생성
	D3DXQUATERNION Qur{};
	D3DXQuaternionRotationYawPitchRoll(&Qur, vAngle.y, vAngle.x, vAngle.z);

	Set_State(STATE_RIGHT, RotateVectorByQuaternion(vRight, Qur));
	Set_State(STATE_UP, RotateVectorByQuaternion(vUp, Qur));
	Set_State(STATE_LOOK, RotateVectorByQuaternion(vLook, Qur));
}

void CTransform::Quaternion_Revolution(const _float3& vAxis, const _float3& vCenter, _float fAngle)
{
	_float3 vPos = Get_State(STATE_POSITION);
	Set_State(STATE_POSITION, vPos - vCenter);

	// 사원수 회전 생성
	D3DXQUATERNION Qur{};
	D3DXQuaternionRotationAxis(&Qur, &vAxis, fAngle);

	D3DXMATRIX matRotation;
	D3DXMatrixRotationQuaternion(&matRotation, &Qur);

	m_WorldMatrix *= matRotation;

	vPos = Get_State(STATE_POSITION);
	Set_State(STATE_POSITION, vPos + vCenter);
}


void CTransform::Quaternion_Revolution_Pos(const _float3& vAxis, const _float3& vCenter, _float fAngle)
{
	_float3 vPos = Get_State(STATE_POSITION);

	// 사원수 회전 생성
	D3DXQUATERNION Qur{};
	D3DXQuaternionRotationAxis(&Qur, &vAxis, fAngle);

	vPos -= vCenter; // 1. 중심을 원점으로 이동
	vPos = RotateVectorByQuaternion(vPos, Qur); // 2. 회전
	vPos += vCenter;

	Set_State(STATE_POSITION, vPos);
}

inline _float3 CTransform::RotateVectorByQuaternion(const _float3& v, const D3DXQUATERNION& q)
{
	D3DXQUATERNION qVec{ v.x, v.y, v.z, 0.0f };
	D3DXQUATERNION qConj;
	D3DXQuaternionInverse(&qConj, &q); // 켤레(역 사원수)

	// 회전 연산: v' = q * v * q^-1
	D3DXQUATERNION qResult, temp;
	D3DXQuaternionMultiply(&temp, &q, &qVec);
	D3DXQuaternionMultiply(&qResult, &temp, &qConj);

	return _float3{ qResult.x, qResult.y, qResult.z };
}

_float2 CTransform::WorldToScreen()
{
	// 최종 스크린 좌표.
	_float2 screenPosision;

	// 투영에 w 값 가져오기 위한 4열 변수.
	_float4 screenPos4;
	
	// 현재 자신의 월드 포지션.
	_float3 worldPosition = Get_State(CTransform::STATE_POSITION);

	_float4x4 viewMat;			// 뷰 행렬.
	_float4x4 projectionMat;	// 투영 행렬.	

	// 행렬 얻어옴.
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &viewMat);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &projectionMat);
	
	// 월드 -> 뷰로 변환.
	D3DXVec3TransformCoord((D3DXVECTOR3*)&screenPos4, &worldPosition, &viewMat);

	// 뷰 -> 투영 변환하고 w값 얻어옴.
	D3DXVec4Transform(&screenPos4, &screenPos4, &projectionMat);

	// 스크린으로 끌고오기?라고 해야하나 암튼
	// w 나누기를 수행하여 원근감을 없앤다.
	// 이것이 뷰포트이다.
	screenPos4.x /= screenPos4.w;
	screenPos4.y /= screenPos4.w;	

	// 뷰포트를 최종 스크린 좌표료 변환.
	screenPos4.x = (screenPos4.x + 1.f) * 0.5f * 1280.f;
	screenPos4.y = (-screenPos4.y + 1.f) * 0.5f * 720.f;

	// 이것이 아름다운 과정을 거쳐서 나온 스크린 좌표다.
	screenPosision = { screenPos4.x, screenPos4.y };

	return screenPosision;
}


CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform* pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();


}
