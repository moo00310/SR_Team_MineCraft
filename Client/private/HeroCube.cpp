#include "HeroCube.h"

#include "GameInstance.h"

CHeroCube::CHeroCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CHeroCube::CHeroCube(const CHeroCube& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CHeroCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHeroCube::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(static_cast<_float>(rand() % 10), 0.5f, static_cast<_float>(rand() % 10)));

	return S_OK;
}

void CHeroCube::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(CCollider_Manager::COLLISION_PLAYER, this);
}

void CHeroCube::Update(_float fTimeDelta)
{
	if (FAILED(m_pColliderCom->Update_ColliderBox()))
	{
		MSG_BOX("Update_ColliderBox()");
		return;
	}

	m_bHit = m_pGameInstance->Collision_with_Group(CCollider_Manager::COLLISION_PLAYER, this, CCollider_Manager::COLLSIION_BOX);

	_float Min;
	_bool b{ false };
	b = m_pGameInstance->IntersectRayOBB(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_LOOK), Min, CCollider_Manager::COLLISION_BLOCK);
	if (b)
	{
 		int a = 10;
	}


	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}
		
}

void CHeroCube::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CHeroCube::Render()
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

	m_pGameInstance->Ray_Cast(m_pTransformCom->Get_WorldMatrix(), m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_LOOK), 100.f, CCollider_Manager::COLLISION_BLOCK);

	return S_OK;
}

HRESULT CHeroCube::Ready_Components()
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

CHeroCube* CHeroCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHeroCube* pInstance = new CHeroCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHeroCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHeroCube::Clone(void* pArg)
{
	CHeroCube* pInstance = new CHeroCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CHeroCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHeroCube::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	//Safe_Release(m_pGraphic_Device); //안돼 이건 억지야

}
