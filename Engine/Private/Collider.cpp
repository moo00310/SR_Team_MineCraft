#include "Collider.h"
#include "Transform.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CComponent{ pGraphic_Device }
{
}

CCollider::CCollider(const CCollider& Prototype)
	:CComponent(Prototype)
	, m_pVB(Prototype.m_pVB)
	, m_pIB(Prototype.m_pIB)
	, m_iNumVertices(Prototype.m_iNumVertices)
	, m_dwFVF(Prototype.m_dwFVF)
	, m_iStride(Prototype.m_iStride)
	, m_ePrimitiveType(Prototype.m_ePrimitiveType)
	, m_iNumPrimitive(Prototype.m_iNumPrimitive)
{

}

HRESULT CCollider::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CCollider::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

HRESULT CCollider::Update_Collider()
{
	return E_NOTIMPL;
}

HRESULT CCollider::Render_Collider(_bool isHit)
{
	return E_NOTIMPL;
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
