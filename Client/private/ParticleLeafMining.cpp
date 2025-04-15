#include "ParticleLeafMining.h"

CParticleLeafMining::CParticleLeafMining(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CBaseParticleMining(pGraphic_Device)
{
}

CParticleLeafMining::CParticleLeafMining(const CParticleLeafMining& Prototype) :
	CBaseParticleMining(Prototype)
{
}

CGameObject* CParticleLeafMining::Clone(void* pArg)
{
	CParticleLeafMining* pInstance = new CParticleLeafMining(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleLeafMining");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleLeafMining* CParticleLeafMining::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleLeafMining* pInstance = new CParticleLeafMining(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleLeafMining");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleLeafMining::OnPushPool()
{
	__super::OnPushPool();
	iCurrentTextureIndex = kMINING::MINING_LEAF;
}

HRESULT CParticleLeafMining::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_MINING,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform))))
		return E_FAIL;
}
