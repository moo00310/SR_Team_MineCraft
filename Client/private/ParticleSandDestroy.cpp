#include "ParticleSandDestroy.h"

CParticleSandDestroy::CParticleSandDestroy(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CBaseParticleDestroy(pGraphic_Device)
{
}

CParticleSandDestroy::CParticleSandDestroy(const CParticleSandDestroy& Prototype) :
	CBaseParticleDestroy(Prototype)
{
}

CGameObject* CParticleSandDestroy::Clone(void* pArg)
{
	CParticleSandDestroy* pInstance = new CParticleSandDestroy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleSandDestroy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleSandDestroy* CParticleSandDestroy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleSandDestroy* pInstance = new CParticleSandDestroy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleSandDestroy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CParticleSandDestroy::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_SAND_MINING,
		TEXT("Com_Component_Sand_Mining"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	return S_OK;
}
