#include "DestroyCube.h"

CDestroyCube::CDestroyCube(LPDIRECT3DDEVICE9 pGraphic_Device) :
	CBreakableCube(pGraphic_Device)
{

}

CDestroyCube::CDestroyCube(const CDestroyCube& Prototype) :
	CBreakableCube(Prototype)
{
}

HRESULT CDestroyCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDestroyCube::Initialize(void* pArg)
{
	return S_OK;
}

void CDestroyCube::Priority_Update(_float fTimeDelta)
{
}

void CDestroyCube::Update(_float fTimeDelta)
{
}

void CDestroyCube::Late_Update(_float fTimeDelta)
{
}

HRESULT CDestroyCube::Render()
{
	return S_OK;
}

HRESULT CDestroyCube::Ready_Components()
{
	return S_OK;
}

CDestroyCube* CDestroyCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CDestroyCube* pInstance = new CDestroyCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDestroyCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDestroyCube::Clone(void* pArg)
{
	CDestroyCube* pInstance = new CDestroyCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CDestroyCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDestroyCube::Free()
{
	__super::Free();
}
