#include "VIBuffer_RectShader.h"

D3DVERTEXELEMENT9 vertexRect[] =
{
	// 정점 버퍼 (Stream 0)
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // vPosition
	{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, // vTexcoord
	D3DDECL_END()
};

CVIBuffer_RectShader::CVIBuffer_RectShader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer { pGraphic_Device }
{
}

CVIBuffer_RectShader::CVIBuffer_RectShader(const CVIBuffer_RectShader& Prototype)
	: CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_RectShader::Initialize_Prototype()
{

	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_iNumPritimive = 2;

	m_iIndexStride = 2;
	m_iNumIndices = 6;
	m_eIndexFormat = D3DFMT_INDEX16;

#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };


	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);		

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

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

	m_pIB->Unlock();
#pragma endregion

	HRESULT hr = m_pGraphic_Device->CreateVertexDeclaration(vertexRect, &pVertexDecl);
	if (FAILED(hr) || pVertexDecl == nullptr)
	{
		MessageBox(0, L"Failed to create Vertex Declaration!", L"Error", MB_OK);
	}

	return S_OK;
}

HRESULT CVIBuffer_RectShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_RectShader::Bind_Buffers()
{
	if (!m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetVertexDeclaration(pVertexDecl);
	// 정점 버퍼 바인딩 (기본 버퍼)
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);

	// 인덱스 버퍼 설정
	m_pGraphic_Device->SetIndices(m_pIB);

	return S_OK;
}

HRESULT CVIBuffer_RectShader::Render()
{
	HRESULT hr = m_pGraphic_Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,  // 삼각형 리스트
		0,                   // 정점 시작 인덱스
		0,                   // 최소 정점 인덱스
		m_iNumVertices,      // 총 정점 개수
		0,                   // 인덱스 버퍼 오프셋
		m_iNumPritimive      // 삼각형 개수
	);

	m_pGraphic_Device->SetStreamSourceFreq(0, 1);

	return S_OK;
}

CVIBuffer_RectShader* CVIBuffer_RectShader::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_RectShader* pInstance = new CVIBuffer_RectShader(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_RectShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_RectShader::Clone(void* pArg)
{
	CVIBuffer_RectShader* pInstance = new CVIBuffer_RectShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_RectShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_RectShader::Free()
{
	__super::Free();
	Safe_Release(pVertexDecl);
}

