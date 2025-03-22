#include "VIBuffer_Rect3D.h"

CVIBuffer_Rect3D::CVIBuffer_Rect3D(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_Rect3D::CVIBuffer_Rect3D(const CVIBuffer_Rect3D& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Rect3D::Initialize_Prototype()
{

	m_iNumVertices = 8;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_iNumPritimive = 4;

	m_iIndexStride = 2;
	m_iNumIndices = 12;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };


	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);		

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexcoord = _float2(0.f, 0.f);

	pVertices[5].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[5].vTexcoord = _float2(1.f, 0.f);

	pVertices[6].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[6].vTexcoord = _float2(1.f, 1.f);

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexcoord = _float2(0.f, 1.f);

	m_pVB->Unlock();
#pragma endregion

#pragma region INDEX_BUFFER
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	pIndices[6] = 4;
	pIndices[7] = 5;
	pIndices[8] = 6;

	pIndices[9] = 4;
	pIndices[10] = 6;
	pIndices[11] = 7;

	m_pIB->Unlock();
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect3D::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Rect3D* CVIBuffer_Rect3D::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect3D* pInstance = new CVIBuffer_Rect3D(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect3D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Rect3D::Clone(void* pArg)
{
	CVIBuffer_Rect3D* pInstance = new CVIBuffer_Rect3D(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect3D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect3D::Free()
{
	__super::Free();


}
