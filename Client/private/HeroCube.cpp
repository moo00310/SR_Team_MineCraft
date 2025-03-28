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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(static_cast<_float>(rand() % 10), 100.5f, static_cast<_float>(rand() % 10)));

	return S_OK;
}

void CHeroCube::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(COLLISION_PLAYER, this);

	if (GetKeyState('R') & 0x8000)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(static_cast<_float>(rand() % 5), 0.f, 0.f));
		m_pTransformCom->Rotation({ 0.f, 1.f, 1.f }, D3DXToRadian(0.f));
	}

	if (m_pGameInstance->Key_Pressing('W'))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing('S'))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing('A'))
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);
	}
	if (m_pGameInstance->Key_Pressing('D'))
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}

	if (m_pGameInstance->Key_Pressing(VK_SPACE))
	{
		m_pRigidbodyCom->Jump();
	}
}

void CHeroCube::Update(_float fTimeDelta)
{
	_float fRange;
	_bool isRayHit{ false };
	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	isRayHit = m_pGameInstance->Ray_Cast(vPosition, vLook, 10.f, COLLISION_BLOCK, fRange);
	if (isRayHit)
	{
		int a = 10;
	}

	if (FAILED(m_pColliderCom->Update_Collider()))
	{
		MSG_BOX("Update_Collider()");
		return;
	}

	_float3 vDist;

	if (m_pGameInstance->Collision_Check_with_Group(COLLISION_BLOCK, m_pColliderCom, CCollider_Manager::COLLSIION_CUBE, &vDist))
	{
		m_bHit = true;
	}

	if (m_bHit)
	{
		int a = 10;
	}

		
	m_pRigidbodyCom->Update(fTimeDelta, COLLISION_BLOCK);
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

	if (FAILED(m_pColliderCom->Render_Collider(m_bHit)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHeroCube::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_Texture_BackGround"),
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
	CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
	Desc.fRadiusX = 1.f; Desc.fRadiusY = 1.f; Desc.fRadiusZ = 1.f;
	Desc.pTransformCom = m_pTransformCom;
	Desc.pOwner = this;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	/* For.Com_Rigidbody */
	CRigidbody::RIGIDBODY_DESC	RigidbodyDesc{ m_pTransformCom, m_pColliderCom, 0.01f };
	if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_Rigidbody"),
		TEXT("Com_Rigidbody"), reinterpret_cast<CComponent**>(&m_pRigidbodyCom), &RigidbodyDesc)))
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

	Safe_Release(m_pRigidbodyCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
