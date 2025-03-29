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

	//Collider �־�ǰ� ��� �ǰ�
	m_pCollider = Desc.pCollider;
	if (m_pCollider)
	{
		Safe_AddRef(m_pCollider);
	}

	m_fMass = Desc.fMass;


	return S_OK;
}

//#include <algorithm> // std::clamp ���

HRESULT CRigidbody::Update(_float fTimeDelta, _uint iCollsionGroup)
{
	// ��ŸŸ���� �ʹ� ũ�� ���� (��: 0.2f �̻��� ���)
	const _float MAX_DELTA_TIME = 0.2f;
	if (fTimeDelta > MAX_DELTA_TIME)
	{
		return S_OK;  // ��ŸŸ���� �ʹ� ũ�� ó������ �ʰ� ����
	}

	/*�� �̰͵� �浹���� üũ�ϰ� �������� �κ��� �ڷ� �ΰ� ������ ������ �ȵǼ�... �ϴ� ����*/

	m_isGround = false;

	Fall_With_Gravity(fTimeDelta);

	// �浹 �˻�
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

			// ���� ��ġ ���� (�ִ� ��0.5f ����)
			_float3 vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

			//const _float fClamp = 0.5f;//�ǹ� ���� ��???

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

	Compute_Velocity(fTimeDelta);

	return S_OK;
}

HRESULT CRigidbody::Update_RayCast(_float fTimeDelta, _uint iCollsionGroup, _float fRayDist)
{
	m_isGround = false;

	_float fDist{ 0.f };
	CGameObject* pGameObject{ nullptr };
	//�����ɽ�Ʈ�� �� �˻�
	pGameObject = m_pGameInstance->Ray_Cast(m_pTransform->Get_State(CTransform::STATE_POSITION), _float3(0.f, -1.f, 0.f), fRayDist, iCollsionGroup, fDist);
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

	//�����ɽ�Ʈ�� �� �˻�
	pGameObject = m_pGameInstance->Ray_Cast_InstancingObject(m_pTransform->Get_State(CTransform::STATE_POSITION), _float3(0.f, -1.f, 0.f), fRayDist, iCollsionGroup);
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




_bool CRigidbody::Jump()
{
	if (m_isGround) // �ٴڿ� ��� ���� ���� ���� ����
	{
		m_vVelocity.y = m_fJumpForce; // Y�� �������� �� �߰�
		m_isGround = false; // ���߿� �� �ִ� ���·� ����

		return true;
	}

	return false;
}

void CRigidbody::Fall_With_Gravity(_float fTimeDelta)
{
	// 1. �߷� ���ӵ� ���� (Y�� ����)
	const _float3 GRAVITY = { 0.0f, -9.81f, 0.0f }; // ���� �߷� ���ӵ� (����: m/s^2)
	 
	// 2. ������ 0�̸� �߷� ���� ���� (���� ���� ����)
	if (m_fMass == 0.0f) return;

	// 3. ���� �߷� �߰� (�߷� ���ӵ� * ����)
	_float3 gravityForce = GRAVITY * m_fMass;

	// 4. ���ӵ� ��� (F = ma -> a = F/m)
	_float3 acceleration = gravityForce / m_fMass;


	// 5. �ӵ� ������Ʈ (v = v + a * dt)
	m_vVelocity.y = m_vVelocity.y + acceleration.y * fTimeDelta;

	// 6. ��ġ ������Ʈ (p = p + v * dt)
	_float3 vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition + m_vVelocity * fTimeDelta);

	
	// 7. �ٴڿ� ����� �� ���� (������ ó�� ����: ��ġ ���� �� �ӵ� ����)
	vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	if (vPosition.y <= 0.0f) // Y���� 0 ���Ϸ� �������ٸ�
	{
		vPosition.y = 0.0f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		  // �ٴڿ� ��Ҵٸ� ��ġ�� 0���� ����

		if(isFalling())
			m_vVelocity.y = 0.0f;   // �ӵ��� 0���� (�ݵ� ���� ����)

		m_isGround = true;
	}
}

void CRigidbody::Compute_Velocity(_float fTimeDelta)
{
	if (fTimeDelta <= 0.f) return; // ��Ÿ Ÿ���� 0�̸� ������� ����

	//�ӵ��� ��ġ ��ȭ�� / �ð�
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
