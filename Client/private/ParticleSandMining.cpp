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

HRESULT CParticleSandMining::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Sand_Mining"),
		TEXT("Com_Component_Sand_Mining"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	return S_OK;
}
