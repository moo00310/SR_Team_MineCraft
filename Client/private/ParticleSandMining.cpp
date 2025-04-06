#include "ParticleSandMining.h"

CParticleSandMining::CParticleSandMining(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CBaseParticleMining(pGraphic_Device)
{
}

CParticleSandMining::CParticleSandMining(const CParticleSandMining& Prototype) :
	CBaseParticleMining(Prototype)
{
}

CGameObject* CParticleSandMining::Clone(void* pArg)
{
	CParticleSandMining* pInstance = new CParticleSandMining(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleSandMining");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleSandMining* CParticleSandMining::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleSandMining* pInstance = new CParticleSandMining(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleSandMining");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleSandMining::OnPushPool()
{
	iCurrentTextureIndex = kMINING::MINING_DIRT;
}

HRESULT CParticleSandMining::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_MINING,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform))))
		return E_FAIL;	

	return S_OK;
}
