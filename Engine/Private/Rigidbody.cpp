﻿#include "Rigidbody.h"
#include "Transform.h"
#include "Collider_Cube.h"
#include "GameInstance.h"

CRigidbody::CRigidbody(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CRigidbody::CRigidbody(const CRigidbody& Prototype)
	: CComponent(Prototype)
{
}

HRESULT CRigidbody::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRigidbody::Initialize(void* pArg)
{
	RIGIDBODY_DESC Desc{ *static_cast<RIGIDBODY_DESC*>(pArg) };

	//Transform
	m_pTransform = Desc.pTransform;
	Safe_AddRef(m_pTransform);
	if (!m_pTransform)
		return E_FAIL;

	//Collider 있어도되고 없어도 되고
	m_pCollider = Desc.pCollider;
	if (m_pCollider)
	{
		Safe_AddRef(m_pCollider);
	}

	m_fMass = Desc.fMass;


	return S_OK;
}

HRESULT CRigidbody::Update(_float fTimeDelta, _uint iCollsionGroup)
{
	const _float MAX_DELTA_TIME = 0.05f;
	if (fTimeDelta > MAX_DELTA_TIME)
		return S_OK;

	_float3 vOriginalPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vNextPosition = vOriginalPos + m_vVelocity * fTimeDelta; // 예측 위치
	m_isGround = false;
	m_pTransform->Set_State(CTransform::STATE_POSITION, vNextPosition);

	// === 충돌 검사 ===
	if (m_pCollider)
	{
		list<CCollider_Cube::COLLISION_INFO> Collision_Infos;
		_bool isHit = m_pGameInstance->Collision_Check_Group_Multi(iCollsionGroup, Collision_Infos, m_pCollider, CCollider_Manager::COLLSIION_CUBE);

		if (isHit)
		{
			_float fMinX{ 0 }, fMaxX{ 0 };
			_float fMinY{ 0 }, fMaxY{ 0 };
			_float fMinZ{ 0 }, fMaxZ{ 0 };

			for (const auto& tInfo : Collision_Infos)
			{
				switch (tInfo.eCollisionDir)
				{
				case CCollider_Cube::COLLISION_DIR::UP:
					fMinY = min(fMinY, tInfo.vDepth.y);
					m_isGround = true;
					m_vVelocity.y = 0.0f;
					break;
				case CCollider_Cube::COLLISION_DIR::DOWN:
					fMaxY = max(fMaxY, tInfo.vDepth.y);
					m_vVelocity.y = 0.0f;
					break;
				case CCollider_Cube::COLLISION_DIR::FRONT:
					fMinZ = min(fMinZ, tInfo.vDepth.z);
					m_vVelocity.z = 0.0f;
					break;
				case CCollider_Cube::COLLISION_DIR::BACK:
					fMaxZ = max(fMaxZ, tInfo.vDepth.z);
					m_vVelocity.z = 0.0f;
					break;
				case CCollider_Cube::COLLISION_DIR::LEFT:
					fMaxX = max(fMaxX, tInfo.vDepth.x);
					m_vVelocity.x = 0.0f;
					break;
				case CCollider_Cube::COLLISION_DIR::RIGHT:
					fMinX = min(fMinX, tInfo.vDepth.x);
					m_vVelocity.x = 0.0f;
					break;
				}
			}

			// 위치 보정
			vNextPosition.x -= (fMinX + fMaxX);
			vNextPosition.y -= (fMinY + fMaxY);
			vNextPosition.z -= (fMinZ + fMaxZ);

			m_pTransform->Set_State(CTransform::STATE_POSITION, vNextPosition);
		}
	}

	m_vVelocity.y += (-GRAVITY * fTimeDelta); // 중력 적용


#pragma region TEST
	// 바닥을 뚫고 내려가지 않도록 보정
	if (vNextPosition.y < -15.f)
	{
		m_isGround = true;
		m_vVelocity.y = 0.0f;
	}
#pragma endregion

	// 5. 최종 위치 업데이트
	//m_pTransform->Set_State(CTransform::STATE_POSITION, vNextPosition);

	// 7. 넉백 처리
	if (m_isGround && m_isKnockBack)
	{
		m_vVelocity.x = 0.f;
		m_vVelocity.z = 0.f;
		m_isKnockBack = false;
	}

	Compute_Velocity(fTimeDelta);

	return S_OK;
}



HRESULT CRigidbody::Update_RayCast(_float fTimeDelta, _uint iCollsionGroup, _float fRayDist)
{
	m_isGround = false;

	_float3 vOriginalPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vNextPosition = vOriginalPos + m_vVelocity * fTimeDelta; // 예측 위치

	m_pTransform->Set_State(CTransform::STATE_POSITION, vNextPosition);

	CGameObject* pGameObject{ nullptr };
	//레이케스트로 땅 검사
	pGameObject = m_pGameInstance->Ray_Cast(m_pTransform->Get_State(CTransform::STATE_POSITION), _float3(0.f, -1.f, 0.f), fRayDist, iCollsionGroup);
	if (pGameObject)
	{
		m_isGround = true;
		m_vVelocity.y = 0.f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, vOriginalPos);
	}
	else
	{
		m_vVelocity.y += (-GRAVITY * fTimeDelta); // 중력 적용
	}

	// 7. 넉백 처리
	if (m_isGround && m_isKnockBack)
	{
		m_vVelocity.x = 0.f;
		m_vVelocity.z = 0.f;
		m_isKnockBack = false;
	}

	return S_OK;
}

HRESULT CRigidbody::Update_RayCast_InstancingObject(_float fTimeDelta, _uint iCollsionGroup, _float fRayDist)
{
	m_isGround = false;

	_float3 vOriginalPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vNextPosition = vOriginalPos + m_vVelocity * fTimeDelta * m_fMass; // 예측 위치

	m_pTransform->Set_State(CTransform::STATE_POSITION, vNextPosition);

	CGameObject* pGameObject{ nullptr };

	//레이케스트로 땅 검사
	pGameObject = m_pGameInstance->Ray_Cast_InstancedObjects(m_pTransform->Get_State(CTransform::STATE_POSITION), _float3(0.f, -1.f, 0.f), fRayDist, iCollsionGroup);
	if (pGameObject)
	{
		m_isGround = true;
		m_vVelocity.y = 0.f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, vOriginalPos);
	}
	else
	{
		m_vVelocity.y += (-GRAVITY * fTimeDelta); // 중력 적용
	}

	// 7. 넉백 처리
	if (m_isGround && m_isKnockBack)
	{
		m_vVelocity.x = 0.f;
		m_vVelocity.z = 0.f;
		m_isKnockBack = false;
	}

	return S_OK;
}

void CRigidbody::Move(_float3 vDir)
{
	D3DXVec3Normalize(&vDir, &vDir);

	m_vVelocity.x = vDir.x * m_fSpeed;
	m_vVelocity.z = vDir.z * m_fSpeed;

	ClampVelocityXZ();
}

void CRigidbody::Chase(const _float3& vTargetPos, _float fMinDistance)
{
	_float3 vDir = vTargetPos - m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float fDistance = D3DXVec3Length(&vDir);
	if (fDistance < fMinDistance)
	{
		m_vVelocity.x = 0.f;
		m_vVelocity.z = 0.f;
		return;
	}
	D3DXVec3Normalize(&vDir, &vDir);
	m_vVelocity.x = vDir.x * m_fSpeed;
	m_vVelocity.z = vDir.z * m_fSpeed;
	ClampVelocityXZ();
}

void CRigidbody::Go_Straight()
{
	_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	//미끄러짐
	//m_vVelocity += vLook * m_fMoveSpeed;

	// 기존 속도 제거 후 새로운 속도 적용
	m_vVelocity.x = vLook.x * m_fSpeed;
	m_vVelocity.z = vLook.z * m_fSpeed;

	ClampVelocityXZ();
}

void CRigidbody::Go_Backward()
{
	_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	//미끄러짐
	//m_vVelocity += vLook * m_fMoveSpeed;

	// 기존 속도 제거 후 새로운 속도 적용
	m_vVelocity.x = -vLook.x * m_fSpeed;
	m_vVelocity.z = -vLook.z * m_fSpeed;

	ClampVelocityXZ();
}

void CRigidbody::Go_Left()
{
	_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);

	//미끄러짐
	//m_vVelocity += vLook * m_fMoveSpeed;

	// 기존 속도 제거 후 새로운 속도 적용
	m_vVelocity.x = -vRight.x * m_fSpeed;
	m_vVelocity.z = -vRight.z * m_fSpeed;

	ClampVelocityXZ();
}

void CRigidbody::Go_Right()
{
	_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	D3DXVec3Normalize(&vRight, &vRight);

	//미끄러짐
	//m_vVelocity += vLook * m_fMoveSpeed;

	// 기존 속도 제거 후 새로운 속도 적용
	m_vVelocity.x = vRight.x * m_fSpeed;
	m_vVelocity.z = vRight.z * m_fSpeed;

	ClampVelocityXZ();
}

void CRigidbody::StopMovement()
{
	m_vVelocity.x = 0.f;
	m_vVelocity.z = 0.f;
}


void CRigidbody::ClampVelocity()
{
	_float speed = D3DXVec3Length(&m_vVelocity);

	if (speed > m_fSpeed)
	{
		D3DXVec3Normalize(&m_vVelocity, &m_vVelocity);
		m_vVelocity *= m_fSpeed;
	}
}

void CRigidbody::ClampVelocityXZ()
{
	// XZ 평면에서의 속도 벡터 계산
	_float2 vVelocityXZ(m_vVelocity.x, m_vVelocity.z);

	_float speedXZ = D3DXVec2Length(&vVelocityXZ);

	if (speedXZ > m_fSpeed)
	{
		D3DXVec2Normalize(&vVelocityXZ, &vVelocityXZ);
		vVelocityXZ *= m_fSpeed;

		// XZ 속도만 조정
		m_vVelocity.x = vVelocityXZ.x;
		m_vVelocity.z = vVelocityXZ.y;
	}
}

void CRigidbody::Compute_Velocity(_float fTimeDelta)
{
	if (fTimeDelta <= 0.f) return; // 델타 타임이 0이면 계산하지 않음

	//속도는 위치 변화량 / 시간
	_float3 vCurrentPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	m_vReadOnly_Velocity = (vCurrentPosition - m_vPrevPosition) / fTimeDelta;

	//printf_s("velocity: %f, %f, %f\n", m_vReadOnly_Velocity.x, m_vReadOnly_Velocity.y, m_vReadOnly_Velocity.z);

	m_vPrevPosition = vCurrentPosition;
}


_bool CRigidbody::Jump(_float fJumpForce)
{
	if (m_isGround) // 바닥에 닿아 있을 때만 점프 가능
	{
		m_vVelocity.y = fJumpForce; // Y축 방향으로 힘 추가
		m_isGround = false; // 공중에 떠 있는 상태로 변경
		//m_isJump = true;
		return true;
	}

	return false;
}

void CRigidbody::Knock_back(const _float3& vfroce)
{
	if (!m_isKnockBack) 
	{
		m_vVelocity.x += vfroce.x;
		m_vVelocity.y += vfroce.y;
		m_vVelocity.z += vfroce.z;
		m_isKnockBack = true;
	}
}

//void CRigidbody::Fall_With_Gravity(_float fTimeDelta)
//{
//	
//
//}




CRigidbody* CRigidbody::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CRigidbody* pInstance = new CRigidbody(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRigidbody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRigidbody::Clone(void* pArg)
{
	CRigidbody* pInstance = new CRigidbody(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CRigidbody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRigidbody::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);

	if (m_pCollider)
	{
		Safe_Release(m_pCollider);
	}
}
