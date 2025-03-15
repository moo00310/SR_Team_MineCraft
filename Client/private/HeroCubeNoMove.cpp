#include "HeroCubeNoMove.h"

#include "GameInstance.h"

CHeroCubeNoMove::CHeroCubeNoMove(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CHeroCubeNoMove::CHeroCubeNoMove(const CHeroCubeNoMove& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CHeroCubeNoMove::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHeroCubeNoMove::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(static_cast<_float>(rand() % 10), 0.5f, static_cast<_float>(rand() % 10)));

	return S_OK;
}

void CHeroCubeNoMove::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(CCollider_Manager::COLLISION_BLOCK, this);
}

void CHeroCubeNoMove::Update(_float fTimeDelta)
{
	if (FAILED(m_pColliderCom->Update_ColliderBox()))
	{
		MSG_BOX("Update_ColliderBox()");
		return;
	}

	//마지막에 발사한 놈만 렌더 되네
	//_float fMin;
	//m_pGameInstance->Ray_Cast(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_LOOK), fMin, CCollider_Manager::COLLISION_PLAYER);


	m_bHit = m_pGameInstance->Collision_with_Group(CCollider_Manager::COLLISION_PLAYER, this, CCollider_Manager::COLLSIION_BOX);
}

void CHeroCubeNoMove::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CHeroCubeNoMove::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(m_pColliderCom->Render_ColliderBox(m_bHit)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHeroCubeNoMove::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider_Cube::COLLRECTDESC Desc{}; //콜라이더 크기 설정
	Desc.fRadiusX = 1.f; Desc.fRadiusY = 1.f; Desc.fRadiusZ = 1.f;
	Desc.pTransformCom = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_CCollider_Cube"),
		TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

CHeroCubeNoMove* CHeroCubeNoMove::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHeroCubeNoMove* pInstance = new CHeroCubeNoMove(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHeroCubeNoMove");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHeroCubeNoMove::Clone(void* pArg)
{
	CHeroCubeNoMove* pInstance = new CHeroCubeNoMove(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CHeroCubeNoMove");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHeroCubeNoMove::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	//Safe_Release(m_pGraphic_Device); //안돼 이건 억지야

}
