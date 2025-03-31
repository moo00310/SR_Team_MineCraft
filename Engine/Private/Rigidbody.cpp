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

//#include <algorithm> // std::clamp 사용

HRESULT CRigidbody::Update(_float fTimeDelta, _uint iCollsionGroup)
{
	// 델타타임이 너무 크면 리턴 (예: 0.2f 이상일 경우)
	const _float MAX_DELTA_TIME = 0.2f;
	if (fTimeDelta > MAX_DELTA_TIME)
	{
		return S_OK;  // 델타타임이 너무 크면 처리하지 않고 리턴
	}

	/*아 이것도 충돌먼저 체크하고 떨어지는 부분을 뒤로 두고 싶은데 점프가 안되서... 일단 냅둠*/

	m_isGround = false;

	Fall_With_Gravity(fTimeDelta);

	// 충돌 검사
	if (m_pCollider)
	{
		list<CCollider_Cube::COLLISION_INFO> Collision_Infos;
		_bool isHit = m_pGameInstance->Collision_Check_Group_Multi(iCollsionGroup, Collision_Infos, m_pCollider, CCollider_Manager::COLLSIION_CUBE);

		if (isHit)
		{
			_float fMaxDepth_Y = 0.0f, fMinDepth_Y = 0.0f;
			_float fMaxDepth_X = 0.0f, fMinDepth_X = 0.0f;
			_float fMaxDepth_Z = 0.0f, fMinDepth_Z = 0.0f;

			for (CCollider_Cube::COLLISION_INFO& tCollision_Info : Collision_Infos)
			{
				_float3 vDepth = tCollision_Info.vDepth;
				CCollider_Cube::COLLISION_DIR eDir = tCollision_Info.eCollisionDir;

				if (eDir == CCollider_Cube::COLLISION_DIR::DOWN)
				{
					m_vVelocity.y = 0.0f;
					fMaxDepth_Y = max(fMaxDepth_Y, vDepth.y);
				}

				if (eDir == CCollider_Cube::COLLISION_DIR::UP)
				{
					if (isFalling())
						m_vVelocity.y = 0.0f;

					m_isGround = true;
					fMinDepth_Y = min(fMinDepth_Y, vDepth.y);
				}
				else
				{
					switch (eDir)
					{
					case CCollider_Cube::COLLISION_DIR::LEFT:
						fMaxDepth_X = max(fMaxDepth_X, vDepth.x);
						break;
					case CCollider_Cube::COLLISION_DIR::RIGHT:
						fMinDepth_X = (fMinDepth_X == 0.f) ? vDepth.x : min(fMinDepth_X, vDepth.x);
						break;
					case CCollider_Cube::COLLISION_DIR::FRONT:
						fMaxDepth_Z = max(fMaxDepth_Z, vDepth.z);
						break;
					case CCollider_Cube::COLLISION_DIR::BACK:
						fMinDepth_Z = (fMinDepth_Z == 0.f) ? vDepth.z : min(fMinDepth_Z, vDepth.z);
						break;
					default:
						break;
					}
				}
			}

			_float3 vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

			vPosition.y -= fMinDepth_Y + fMaxDepth_Y;
			vPosition.x -= fMaxDepth_X + fMinDepth_X;
			vPosition.z -= fMaxDepth_Z + fMinDepth_Z;

			m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		}
	}

	if (m_isGround)
	{
		if (m_isKnockBack)
		{
			m_vVelocity.x = 0.f;
			m_vVelocity.z = 0.f;
			m_isKnockBack = false;
		}
	}

	Compute_Velocity(fTimeDelta);

	return S_OK;
}

HRESULT CRigidbody::Update_RayCast(_float fTimeDelta, _uint iCollsionGroup, _float fRayDist)
{
	m_isGround = false;

	CGameObject* pGameObject{ nullptr };
	//레이케스트로 땅 검사
	pGameObject = m_pGameInstance->Ray_Cast(m_pTransform->Get_State(CTransform::STATE_POSITION), _float3(0.f, -1.f, 0.f), fRayDist, iCollsionGroup);
	if (pGameObject)
	{
		m_isGround = true;
		m_vVelocity.y = 0.f;
	}
	else
	{
		Fall_With_Gravity(fTimeDelta);
	}

	Compute_Velocity(fTimeDelta);

	return S_OK;
}

HRESULT CRigidbody::Update_RayCast_InstancingObject(_float fTimeDelta, _uint iCollsionGroup, _float fRayDist)
{
	m_isGround = false;

	CGameObject* pGameObject{ nullptr };

	//레이케스트로 땅 검사
	pGameObject = m_pGameInstance->Ray_Cast_InstancedObjects(m_pTransform->Get_State(CTransform::STATE_POSITION), _float3(0.f, -1.f, 0.f), fRayDist, iCollsionGroup);
	if (pGameObject)
	{
		m_isGround = true;
		m_vVelocity.y = 0.f;
	}
	else
	{
		Fall_With_Gravity(fTimeDelta);
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

void CRigidbody::Fall_With_Gravity(_float fTimeDelta)
{
	// 1. 중력 가속도 정의
	const _float3 GRAVITY = { 0.0f, -20.f, 0.0f };

	// 2. 질량이 0이면 중력 적용 안함
	if (m_fMass == 0.0f) return;

	// 3. 속도 업데이트 (중력 가속도만 적용)
	m_vVelocity.y += GRAVITY.y * fTimeDelta;

	// 4. 위치 업데이트
	_float3 vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition + m_vVelocity * fTimeDelta);

	// 5. 바닥 충돌 처리
	vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	if (vPosition.y <= 0.0f)
	{
		vPosition.y = 0.0f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);

		if (isFalling())
			m_vVelocity.y = 0.0f;

		m_isGround = true;
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
