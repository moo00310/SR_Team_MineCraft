#include "Collider.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent{ pGraphic_Device }
{
}

CCollider::CCollider(const CCollider& Prototype)
	:CComponent(Prototype)
{
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

CComponent* CCollider::Clone(void* pArg)
{
	return nullptr;
}

void CCollider::Free()
{
	__super::Free();
}
