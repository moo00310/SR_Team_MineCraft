#include "ParticleStoneDestroy.h"

CParticleStoneDestroy::CParticleStoneDestroy(LPDIRECT3DDEVICE9 pGraphic_Device) :
    CBaseParticleDestroy(pGraphic_Device)
{
}

CParticleStoneDestroy::CParticleStoneDestroy(const CParticleStoneDestroy& Prototype) :
    CBaseParticleDestroy(Prototype)
{
}

CGameObject* CParticleStoneDestroy::Clone(void* pArg)
{
	CParticleStoneDestroy* pInstance = new CParticleStoneDestroy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CParticleStoneDestroy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticleStoneDestroy* CParticleStoneDestroy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CParticleStoneDestroy* pInstance = new CParticleStoneDestroy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleStoneDestroy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleStoneDestroy::OnPushPool()
{
	iCurrentTextureIndex = kMINING::MINING_STONE;
}

HRESULT CParticleStoneDestroy::Ready_Components()
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
