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

HRESULT CParticleWoodMining::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_WOOD_MINING,
		TEXT("Com_Component_Wood_Mining"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	return S_OK;
}
