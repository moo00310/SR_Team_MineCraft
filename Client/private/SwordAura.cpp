#include "SwordAura.h"
#include "Pawn.h"

CSwordAura::CSwordAura(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CGameObject(pGraphic_Device)
{
}

CSwordAura::CSwordAura(const CSwordAura& Prototype) :
	CGameObject(Prototype)
{
}

CSwordAura* CSwordAura::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSwordAura* pInstance = new CSwordAura(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSwordAura");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSwordAura::Clone(void* pArg)
{
	CSwordAura* pInstance = new CSwordAura(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CSwordAura");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSwordAura::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferRectCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);
}

HRESULT CSwordAura::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSwordAura::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}	

	return S_OK;
}

void CSwordAura::Priority_Update(_float fTimeDelta)
{	
}

void CSwordAura::Update(_float fTimeDelta)
{
	// 전진이 아니라 다운인 이유는
	// 90도 회전 시켜서 나아갸아하기 때문.
	m_pTransformCom->Go_Down(fTimeDelta);	
	
	_float3 pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	CParticleEventManager::Get_Instance()->OnParticle(
		PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_AURA,
		m_pTransformCom
	);	

	CGameObject* pGameObject{ nullptr };

	pGameObject = m_pGameInstance->Collision_Check_with_Group(
		COLLISION_MONSTER,
		m_pColliderCom,
		CCollider_Manager::COLLSIION_CUBE);

	if (pGameObject != nullptr)
	{
		// 몬스터 피격.
		_float3 vForce{ m_pTransformCom->Get_State(CTransform::STATE_LOOK) };
		D3DXVec3Normalize(&vForce, &vForce);
		vForce *= 3.f;
		vForce.y = 4.f;

		static_cast<CPawn*>(pGameObject)->Add_Hp(-25.f);
		static_cast<CPawn*>(pGameObject)->Knock_back(vForce);
		m_fCurrentTime = 0.f;
		SetActive(false);
		return;
	}

	if (m_fCurrentTime >= m_fEndTime)
	{
		m_fCurrentTime = 0.f;
		SetActive(false);
		return;
	}

	m_fCurrentTime += fTimeDelta;
}

void CSwordAura::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
	{
		return;
	}		
}

HRESULT CSwordAura::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferRectCom->Bind_Buffers()))
		return E_FAIL;

	m_pTransformCom->Bind_Resource(m_pShaderCom);
	m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);
	m_pShaderCom->Begin(2);

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferRectCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();

	return S_OK;
}

CTransform* CSwordAura::GetTransform() const
{
	return m_pTransformCom;
}

HRESULT CSwordAura::Ready_Components()
{
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("m_pVIBufferCom_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferRectCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 20.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_YU, PROTOTYPE_COMPONENT_TEXTURE_SWORD_AURA,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Collider_Cube */
	CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
	Desc.vRadius = { 2.5f, .5f, .5f };	
	Desc.pTransformCom = m_pTransformCom;
	Desc.pOwner = this;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider_Rect"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}
