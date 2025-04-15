#include "Pawn.h"
#include "MCTerrain.h"
#include "Exp_Orb.h"

CPawn::CPawn(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CPawn::CPawn(const CPawn& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CPawn::Initialize(void* pArg)
{
	//MCTerrain �����ͼ� ����
	m_pTerrain = static_cast<CMCTerrain*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Terrain"), 0));
	Safe_AddRef(m_pTerrain);

	return E_NOTIMPL;
}

void CPawn::Priority_Update(_float fTimeDelta)
{


}

void CPawn::Update(_float fTimeDelta)
{
	//1. ���� �浹 �� �༮�� ��������
	list<CCollider*> Colliders = m_pTerrain->Active_Near_Chunk_Colliders(m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f, 1.5f, 0.f }, 8.f);

	//2. �浹 ó��
	m_pRigidbodyCom->Update(fTimeDelta, COLLISION_BLOCK);

	//3. ���� �浹 �� �༮�� ����
	for (auto pCollider : Colliders)
	{
		m_pGameInstance->Out_Collider_CollisiomGroup(COLLISION_BLOCK, pCollider);
	}
}

HRESULT CPawn::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	for (int i = 0; i < m_pVIBufferComs.size(); i++)
	{
		if (FAILED(m_pVIBufferComs[i]->Bind_WorldMatrix(m_pShaderCom)))
			return E_FAIL;

		if (FAILED(m_pVIBufferComs[i]->Bind_Buffers()))
			return E_FAIL;

		m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);

		m_pShaderCom->SetFloat("g_Bright", g_fBright + 0.2f);
		m_pShaderCom->Begin(m_eColor);

		/* ������ �׸���. */
		if (FAILED(m_pVIBufferComs[i]->Render()))
			return E_FAIL;

		m_pShaderCom->End();
	}

	if (FAILED(m_pCollider_CubeCom->Render_Collider(true)))
		return E_FAIL;

	return S_OK;
}

void CPawn::Add_Hp(_float damage)
{
	m_Hp += damage;

	if (m_Hp <= 0.f)
	{
		Dead_Pawn();
	}
}

void CPawn::Reset_Ainmation()
{
	m_skelAnime->Set_ZeroAnimTime();
	m_skelAnime->InitBone();
	m_eColor = RENDERORIGIN;
}


void CPawn::Free()
{
	__super::Free();

	if (m_isCloned)
	{
		Safe_Release(m_pTerrain);
	}

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

void CPawn::Dead_Pawn()
{
	isDead = true;
	m_eCurAnim = DEAD;


	//����ġ 3�� ����߸�
	for (_uint i = 0; i < 5; ++i)
	{
		m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Exp_Orb"),
			LEVEL_YU, TEXT("Layer_Exp_Orb"));
		CGameObject* pExp_Orb = m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Exp_Orb"));
		static_cast<CExp_Orb*>(pExp_Orb)->Get_Transform()->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f, 1.f, 0.f });
	}
}
