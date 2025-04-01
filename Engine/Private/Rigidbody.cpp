#include "Rigidbody.h"
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
	// 델타타임이 너무 크면 리턴
	const _float MAX_DELTA_TIME = 0.2f;
	if (fTimeDelta > MAX_DELTA_TIME)
		return S_OK;

	// 1. 예측 위치로 Collider 업데이트
	_float3 vOriginalPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vNextPosition = vOriginalPos + m_vVelocity * fTimeDelta; // 예측 위치

	m_isGround = false;
	m_pTransform->Set_State(CTransform::STATE_POSITION, vNextPosition);

	// 2. 충돌 검사 (예측 위치 기준)
	if (m_pCollider)
	{
		list<CCollider_Cube::COLLISION_INFO> Collision_Infos;
		_bool isHit = m_pGameInstance->Collision_Check_Group_Multi(iCollsionGroup, Collision_Infos, m_pCollider, CCollider_Manager::COLLSIION_CUBE);

		if (isHit)
		{
			_float fMinY{ 0 }; // 충돌 깊이를 찾기 위한 초기값
			_float fMaxY{ 0 };
			for (const auto& tInfo : Collision_Infos)
			{
				if (tInfo.eCollisionDir == CCollider_Cube::COLLISION_DIR::UP)
				{
					fMinY = min(fMinY, tInfo.vDepth.y);
					m_isGround = true;
					m_vVelocity.y = 0.0f; // 속도 초기화
				}

				if (tInfo.eCollisionDir == CCollider_Cube::COLLISION_DIR::DOWN)
				{
					fMaxY = max(fMaxY, tInfo.vDepth.y);
					m_vVelocity.y = 0.f; // 속도 초기화
				}
			}

			vNextPosition.y -= fMinY;
			vNextPosition.y -= fMaxY;
		}
	}

	m_vVelocity.y += (-GRAVITY * fTimeDelta); // 중력 적용

#pragma region TEST
	//이놈은 0.f일 때 못내려가게 해주는거 나중에 없앨만 한놈
	if (vNextPosition.y < 0.f)
	{
		m_isGround = true;
		m_vVelocity.y = 0.0f;
	}
#pragma endregion

	// 5. 최종 위치 업데이트
	m_pTransform->Set_State(CTransform::STATE_POSITION, vNextPosition);

	// 7. 넉백 처리
	if (m_isGround && m_isKnockBack)
	{
		m_vVelocity.x = 0.f;
		m_vVelocity.z = 0.f;
		m_isKnockBack = false;
	}

	Compute_Velocity(fTimeDelta);

	printf_s("isGround: %d\n", m_isGround);

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

	Compute_Velocity(fTimeDelta);

	return S_OK;
}

HRESULT CRigidbody::Update_RayCast_InstancingObject(_float fTimeDelta, _uint iCollsionGroup, _float fRayDist)
{
	m_isGround = false;

	_float3 vOriginalPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	_float3 vNextPosition = vOriginalPos + m_vVelocity * fTimeDelta; // 예측 위치

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

	Compute_Velocity(fTimeDelta);

	return S_OK;
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
		m_vVelocity.x = vfroce.x;
		m_vVelocity.y = vfroce.y;
		m_vVelocity.z = vfroce.z;
		m_isKnockBack = true;
	}
}

//void CRigidbody::Fall_With_Gravity(_float fTimeDelta)
//{
//	
//
//}


void CRigidbody::Compute_Velocity(_float fTimeDelta)
{
	if (fTimeDelta <= 0.f) return; // 델타 타임이 0이면 계산하지 않음

	//속도는 위치 변화량 / 시간
	_float3 vCurrentPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	m_vReadOnly_Velocity = (vCurrentPosition - m_vPrevPosition) / fTimeDelta;

	//printf_s("velocity: %f, %f, %f\n", m_vReadOnly_Velocity.x, m_vReadOnly_Velocity.y, m_vReadOnly_Velocity.z);

	m_vPrevPosition = vCurrentPosition;

}



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
