#include "ParticleStoneMining.h"

CParticleStoneMining::CParticleStoneMining(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CBaseParticleMining(pGraphic_Device)
{
}

CParticleStoneMining::CParticleStoneMining(const CParticleStoneMining& Prototype) :
	CBaseParticleMining(Prototype)
{
}

CGameObject* CParticleStoneMining::Clone(void* pArg)
{
	CParticleStoneMining* pInstance = new CParticleStoneMining(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleStoneMining");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleStoneMining* CParticleStoneMining::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleStoneMining* pInstance = new CParticleStoneMining(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleStoneMining");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleStoneMining::OnPushPool()
{
	iCurrentTextureIndex = kMINING::MINING_STONE;
}

HRESULT CParticleStoneMining::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_MINING,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform))))
		return E_FAIL;

	return S_OK;
}
