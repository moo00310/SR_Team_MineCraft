#include "ParticleLeafDestroy.h"

CParticleLeafDestroy::CParticleLeafDestroy(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CBaseParticleDestroy(pGraphic_Device)
{
}

CParticleLeafDestroy::CParticleLeafDestroy(const CParticleLeafDestroy& Prototype) :
	CBaseParticleDestroy(Prototype)
{
}

CGameObject* CParticleLeafDestroy::Clone(void* pArg)
{
	CParticleLeafDestroy* pInstance = new CParticleLeafDestroy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleLeafDestroy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleLeafDestroy* CParticleLeafDestroy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleLeafDestroy* pInstance = new CParticleLeafDestroy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleLeafDestroy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleLeafDestroy::OnPushPool()
{
	__super::OnPushPool();
	iCurrentTextureIndex = kMINING::MINING_LEAF;
}

HRESULT CParticleLeafDestroy::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_MINING,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pParticleTexture))))
		return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransform), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}
