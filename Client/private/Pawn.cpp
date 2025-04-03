#include "Pawn.h"

CPawn::CPawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CPawn::CPawn(const CPawn& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CPawn::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	for (int i = 0; i < 6; i++)
	{
		if (FAILED(m_pVIBufferComs[i]->Bind_WorldMatrix(m_pShaderCom)))
			return E_FAIL;

		if (FAILED(m_pVIBufferComs[i]->Bind_Buffers()))
			return E_FAIL;

		m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);
		m_pShaderCom->SetFloat("g_Bright", g_fBright + 0.2f);
		m_pShaderCom->Begin(1);

		/* ������ �׸���. */
		if (FAILED(m_pVIBufferComs[i]->Render()))
			return E_FAIL;

		m_pShaderCom->End();
	}


	if (FAILED(m_pCollider_CubeCom->Render_Collider(true)))
		return E_FAIL;

	return S_OK;
}

void CPawn::Reset_Ainmation()
{
	m_skelAnime->Set_ZeroAnimTime();
	m_skelAnime->InitBone();
}


void CPawn::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRigidbodyCom);
	Safe_Release(m_pCollider_CubeCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_skelAnime);
	Safe_Release(m_pTextureCom);

	for (auto& buffer : m_pVIBufferComs)
		Safe_Release(buffer);
}

HRESULT CPawn::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ m_fSpeed, D3DXToRadian(30.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	// �� + �ִϸ��̼�
	CSkeletalAnimator::DESC DescSekel = { m_pVIBufferComs };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SkeletalAnimator"),
		TEXT("m_pSkeletalAnimatorCom"), reinterpret_cast<CComponent**>(&m_skelAnime), &DescSekel)))
		return E_FAIL;

	// ���̴� ������Ʈ
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Cube"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	////�ݶ��̴�
	/* For.Com_Collider */
	CCollider_Cube::COLLCUBE_DESC Desc{}; //�ݶ��̴� ũ�� ����
	Desc.vRadius = m_Coll_Size;
	Desc.vOffset = m_Coll_Offset;
	Desc.pTransformCom = m_pTransformCom;
	Desc.pOwner = this;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pCollider_CubeCom), &Desc)))
		return E_FAIL;

	//������ٵ�
	/* For.Com_Rigidbody */
	CRigidbody::RIGIDBODY_DESC	RigidbodyDesc{ m_pTransformCom, m_pCollider_CubeCom, 1.f };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Rigidbody"),
		TEXT("Com_Rigidbody"), reinterpret_cast<CComponent**>(&m_pRigidbodyCom), &RigidbodyDesc)))
		return E_FAIL;

	return S_OK;
}
