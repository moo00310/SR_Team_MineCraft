#include "Pawn.h"

CPawn::CPawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CPawn::CPawn(const CPawn& Prototype)
	:CGameObject(Prototype)
{
}

void CPawn::Reset_Ainmation()
{
	m_skelAnime->Set_ZeroAnimTime();
	m_skelAnime->InitBone();
}

void CPawn::Knock_back(const _float3& vforce)
{
	_float3 temp = {};
	D3DXVec3Normalize(&temp, &vforce);
	temp *= 3.f;
	temp.y = 4.f;

	m_pRigidbodyCom->Knock_back(temp);

}

void CPawn::Free()
{
	__super::Free();

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

	// 본 + 애니메이션
	CSkeletalAnimator::DESC DescSekel = { m_pVIBufferComs };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SkeletalAnimator"),
		TEXT("m_pSkeletalAnimatorCom"), reinterpret_cast<CComponent**>(&m_skelAnime), &DescSekel)))
		return E_FAIL;

	////콜라이더
	/* For.Com_Collider */
	CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
	Desc.vRadius = { .3f, .8f, .3f };
	Desc.vOffset = { 0.f, 0.8f, 0.f };
	Desc.pTransformCom = m_pTransformCom;
	Desc.pOwner = this;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pCollider_CubeCom), &Desc)))
		return E_FAIL;

	//리지드바디
	/* For.Com_Rigidbody */
	CRigidbody::RIGIDBODY_DESC	RigidbodyDesc{ m_pTransformCom, m_pCollider_CubeCom, 1.f };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Rigidbody"),
		TEXT("Com_Rigidbody"), reinterpret_cast<CComponent**>(&m_pRigidbodyCom), &RigidbodyDesc)))
		return E_FAIL;

	return S_OK;
}
