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
	m_pCollider_Cube = Desc.pCollider_Cube;
	if (m_pCollider_Cube)
	{
		Safe_AddRef(m_pCollider_Cube);
	}

	m_fMass = Desc.fMass;


	return S_OK;
}

#include <algorithm> // std::clamp 사용

HRESULT CRigidbody::Update(_float fTimeDelta, _uint iCollsionGroup)
{
	m_isGrounded = false;

	Fall_With_Gravity(fTimeDelta);

	// 충돌 검사
	if (m_pCollider_Cube)
	{
		list<CCollider_Cube::COLLISION_INFO> Collision_Infos;
		_bool isHit = m_pGameInstance->Collision_Check_Group_Multi(iCollsionGroup, Collision_Infos, m_pCollider_Cube, CCollider_Manager::COLLSIION_CUBE);

		if (isHit)
		{
			_float fMaxDepth_Y = 0.0f, fMinDepth_Y = 0.0f;
			_float fMaxDepth_X = 0.0f, fMinDepth_X = 0.0f;
			_float fMaxDepth_Z = 0.0f, fMinDepth_Z = 0.0f;

			for (CCollider_Cube::COLLISION_INFO& tCollision_Info : Collision_Infos)
			{
				_float3 vDepth = tCollision_Info.vDepth;
				CCollider_Cube::COLLSION_DIR eDir = tCollision_Info.eCollisionDir;

				if (eDir == CCollider_Cube::COLLSION_DIR::DOWN)
				{
					m_vVelocity.y = 0.0f;
					fMaxDepth_Y = max(fMaxDepth_Y, vDepth.y);
				}

				if (eDir == CCollider_Cube::COLLSION_DIR::UP)
				{
					if (isFalling())
						m_vVelocity.y = 0.0f;

					m_isGrounded = true;
					fMinDepth_Y = min(fMinDepth_Y, vDepth.y);
				}
				else
				{
					switch (eDir)
					{
					case CCollider_Cube::COLLSION_DIR::LEFT:
						fMaxDepth_X = max(fMaxDepth_X, vDepth.x);
						break;
					case CCollider_Cube::COLLSION_DIR::RIGHT:
						fMinDepth_X = (fMinDepth_X == 0.f) ? vDepth.x : min(fMinDepth_X, vDepth.x);
						break;
					case CCollider_Cube::COLLSION_DIR::FRONT:
						fMaxDepth_Z = max(fMaxDepth_Z, vDepth.z);
						break;
					case CCollider_Cube::COLLSION_DIR::BACK:
						fMinDepth_Z = (fMinDepth_Z == 0.f) ? vDepth.z : min(fMinDepth_Z, vDepth.z);
						break;
					default:
						break;
					}
				}
			}

			// 현재 위치 보정 (최대 ±0.5f 제한)
			_float3 vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

			//const _float fClamp = 0.5f;//의미 없는 듯???

			//_float fTotalDepth_Y = clamp(fMinDepth_Y + fMaxDepth_Y, -fClamp, fClamp);
			//vPosition.y -= fTotalDepth_Y;
			vPosition.y -= fMinDepth_Y + fMaxDepth_Y;

			/*_float fTotalDepth_X = clamp(fMaxDepth_X + fMinDepth_X, -fClamp, fClamp);
			vPosition.x -= fTotalDepth_X;*/
			vPosition.x -= fMaxDepth_X + fMinDepth_X;

			/*_float fTotalDepth_Z = clamp(fMaxDepth_Z + fMinDepth_Z, -fClamp, fClamp);
			vPosition.z -= fTotalDepth_Z;*/
			vPosition.z -= fMaxDepth_Z + fMinDepth_Z;

			m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		}
	}

	printf_s("velocity: %f, %f, %f\n", m_vVelocity.x, m_vVelocity.y, m_vVelocity.z);

	return S_OK;
}




_bool CRigidbody::Jump()
{
	if (m_isGrounded) // 바닥에 닿아 있을 때만 점프 가능
	{
		m_vVelocity.y = m_fJumpForce; // Y축 방향으로 힘 추가
		m_isGrounded = false; // 공중에 떠 있는 상태로 변경

		return true;
	}

	return false;
}

void CRigidbody::Fall_With_Gravity(_float fTimeDelta)
{
	// 1. 중력 가속도 정의 (Y축 방향)
	const _float3 GRAVITY = { 0.0f, -9.81f, 0.0f }; // 지구 중력 가속도 (단위: m/s^2)
	 
	// 2. 질량이 0이면 중력 적용 안함 (무한 질량 가정)
	if (m_fMass == 0.0f) return;

	// 3. 힘에 중력 추가 (중력 가속도 * 질량)
	_float3 gravityForce = GRAVITY * m_fMass;

	// 4. 가속도 계산 (F = ma -> a = F/m)
	_float3 acceleration = gravityForce / m_fMass;

	// 5. 속도 업데이트 (v = v + a * dt)
	m_vVelocity = m_vVelocity + acceleration * fTimeDelta;

	// 6. 위치 업데이트 (p = p + v * dt)
	_float3 vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition + m_vVelocity * fTimeDelta);

	
	// 7. 바닥에 닿았을 때 반응 (간단한 처리 예시: 위치 보정 및 속도 감쇠)
	vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	if (vPosition.y <= 0.0f) // Y축이 0 이하로 내려갔다면
	{
		vPosition.y = 0.0f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		  // 바닥에 닿았다면 위치를 0으로 고정

		if(isFalling())
			m_vVelocity.y = 0.0f;   // 속도를 0으로 (반동 없이 멈춤)

		m_isGrounded = true;
	}
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

	if (m_pCollider_Cube)
	{
		Safe_Release(m_pCollider_Cube);
	}
}
