#include "VIBuffer_Line.h"

CVIBuffer_Line::CVIBuffer_Line(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Line::CVIBuffer_Line(const CVIBuffer_Line& Prototype)
	:CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Line::Initialize_Prototype()
{
	m_iNumVertices = 2;
	m_iVertexStride = sizeof(VTXPOSCOL);
	m_iFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_iNumPritimive = 2;

	m_iIndexStride = 2;
	m_iNumIndices = 6;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER

#pragma endregion

#pragma region INDEX_BUFFER

#pragma endregion
	return S_OK;
}

HRESULT CVIBuffer_Line::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

CVIBuffer_Line* CVIBuffer_Line::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Line* pInstance = new CVIBuffer_Line(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Line");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Line::Clone(void* pArg)
{
	CVIBuffer_Line* pInstance = new CVIBuffer_Line(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Line");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Line::Free()
{
	__super::Free();

}
