#include "FireCrackerLoad.h"

CFireCrackerLoad::CFireCrackerLoad(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CGameObject(pGraphic_Device)
{
}

CFireCrackerLoad::CFireCrackerLoad(const CFireCrackerLoad& Prototype) :
	CGameObject(Prototype)
{
}

CFireCrackerLoad* CFireCrackerLoad::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFireCrackerLoad* pInstance = new CFireCrackerLoad(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFireCrackerLoad");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFireCrackerLoad::Clone(void* pArg)
{
	CFireCrackerLoad* pInstance = new CFireCrackerLoad(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CFireCrackerLoad");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFireCrackerLoad::Free()
{
	__super::Free();
}

HRESULT CFireCrackerLoad::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFireCrackerLoad::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
	{
		return E_FAIL;
	}

	Safe_Release(m_pTransformCom);

	return S_OK;
}

void CFireCrackerLoad::Priority_Update(_float fTimeDelta)
{
}

void CFireCrackerLoad::Update(_float fTimeDelta)
{
	CParticleEventManager::Get_Instance()->OnParticle(
		PROTOTYPE_GAMEOBJECT_PARTICLE_CRACKER_LOAD,
		m_pTransformCom
	);
	m_pTransformCom->Go_Up(fTimeDelta);

	if (fCurrentCount >= fEndCount)
	{
		CParticleEventManager::Get_Instance()->OnParticle(
			PROTOTYPE_GAMEOBJECT_PARTICLE_FIRE_CRACKER,
			m_pTransformCom
		);

		fCurrentCount = 0.f;
		SetActive(false);
		return;
	}

	fCurrentCount += fTimeDelta;
}

void CFireCrackerLoad::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
	{
		return;
	}
}

HRESULT CFireCrackerLoad::Render()
{
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	return S_OK;
}

void CFireCrackerLoad::OnPushPool()
{
}

CTransform* CFireCrackerLoad::GetTransform() const
{
	return m_pTransformCom;
}

HRESULT CFireCrackerLoad::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}
