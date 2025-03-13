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

	return S_OK;
}

void CHeroCube::Priority_Update(_float fTimeDelta)
{
}

void CHeroCube::Update(_float fTimeDelta)
{
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

	if (FAILED(m_pColliderCom->Update_ColliderBox(m_pTransformCom->Get_WorldMatrix())))
	{
		MSG_BOX("Update_ColliderBox()");
		return;
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

	if (FAILED(m_pColliderCom->Render_ColliderBox()))
		return E_FAIL;

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

	/* For.Com_Collider */
	CCollider_Cube::COLLRECTDESC Desc{};
  	Desc.fRadiusX = 1.f; Desc.fRadiusY = 1.f; Desc.fRadiusZ = 1.f; //콜라이더 크기 결정(왜 이거를 하면 릴리즈 에러가 나는가)
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_CCollider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	/*if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_CCollider_Cube"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom))))
		return E_FAIL;*/

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
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
