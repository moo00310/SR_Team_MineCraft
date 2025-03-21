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
	m_pCollider_Cube = Desc.pCollider_Cube;
	if (m_pCollider_Cube)
	{
		Safe_AddRef(m_pCollider_Cube);
	}

	m_fMass = Desc.fMass;


	return S_OK;
}

HRESULT CRigidbody::Update(_float fTimeDelta, _uint iCollsionGroup)
{
	m_isGrounded = false;

	Fall_With_Gravity(fTimeDelta);

	// �浹 �˻�
	if (m_pCollider_Cube)
	{
		_float3 vDist;
		CCollider_Cube::COLLSION_DIR eDir;
		_bool isHit = m_pGameInstance->Collision_with_Group(iCollsionGroup, m_pCollider_Cube, CCollider_Manager::COLLSIION_BOX, &vDist, &eDir);

		if (isHit)
		{
			// ���� ��ġ ���� (Y���� �ٴ� �浹�� ���� ����)
			_float3 vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

			if (eDir == CCollider_Cube::COLLSION_DIR::UP)
			{
				vPosition.y -= vDist.y;  // �ٴ� �浹 �� Y�� ����
				m_vVelocity.y = 0.0f;    // �ӵ� ����
				m_isGrounded = true;
			}
			else
			{
				vPosition.x -= vDist.x;  // �� �浹 �� X�� ����
				vPosition.z -= vDist.z;  // �� �浹 �� Z�� ����
			}

			m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		}
	}

	if (m_isGrounded)
	{
		//printf_s("is Ground\n");
	}
	else
	{
		//printf_s("is Air\n");
	}


	return S_OK;
}



void CRigidbody::Jump()
{
	if (m_isGrounded) // �ٴڿ� ��� ���� ���� ���� ����
	{
		m_vVelocity.y = m_fJumpForce; // Y�� �������� �� �߰�
		m_isGrounded = false; // ���߿� �� �ִ� ���·� ����
	}
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
	m_vVelocity = m_vVelocity + acceleration * fTimeDelta;

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
		m_vVelocity.y = 0.0f;   // �ӵ��� 0���� (�ݵ� ���� ����)
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
