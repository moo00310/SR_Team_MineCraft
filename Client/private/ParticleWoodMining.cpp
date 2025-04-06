#include "ParticleWoodMining.h"

CParticleWoodMining::CParticleWoodMining(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CBaseParticleMining(pGraphic_Device)
{
}

CParticleWoodMining::CParticleWoodMining(const CParticleWoodMining& Prototype) :
	CBaseParticleMining(Prototype)
{
}

CGameObject* CParticleWoodMining::Clone(void* pArg)
{
	CParticleWoodMining* pInstance = new CParticleWoodMining(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleWoodMining");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleWoodMining* CParticleWoodMining::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleWoodMining* pInstance = new CParticleWoodMining(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleWoodMining");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleWoodMining::OnPushPool()
{
	iCurrentTextureIndex = kMINING::MINING_WOOD;
}

HRESULT CParticleWoodMining::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_MINING,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform))))
		return E_FAIL;

	iCurrentTextureIndex = 1;

	return S_OK;
}
