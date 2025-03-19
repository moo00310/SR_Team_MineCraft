#include "Rigidbody.h"
#include "Transform.h"

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
	m_pTransform = Desc.pTransform;
	Safe_AddRef(m_pTransform);
	if (!m_pTransform)
		return E_FAIL;
	m_fMass = Desc.fMass;


	return S_OK;
}

HRESULT CRigidbody::Update(_float fTimeDelta)
{
	Fall_With_Gravity(fTimeDelta);

	return S_OK;
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

	vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);
	// 7. �ٴڿ� ����� �� ���� (������ ó�� ����: ��ġ ���� �� �ӵ� ����)
	if (vPosition.y <= 0.0f) // Y���� 0 ���Ϸ� �������ٸ�
	{
		vPosition.y = 0.0f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		  // �ٴڿ� ��Ҵٸ� ��ġ�� 0���� ����
		m_vVelocity.y = 0.0f;   // �ӵ��� 0���� (�ݵ� ���� ����)
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
}
