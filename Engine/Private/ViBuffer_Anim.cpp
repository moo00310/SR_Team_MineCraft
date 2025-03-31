#include "ViBuffer_Anim.h"

CVIBuffer_Anim::CVIBuffer_Anim(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Anim::CVIBuffer_Anim(const CVIBuffer_Anim& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Anim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Anim::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Anim::Bind_WorldMatrix()
{
	m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
	return S_OK;
}

void CVIBuffer_Anim::SetMatrix(const D3DXMATRIX& mat)
{
	m_WorldMatrix = mat;
}

CComponent* CVIBuffer_Anim::Clone(void* pArg)
{
	return nullptr;
}

void CVIBuffer_Anim::Free()
{
	__super::Free();
}
