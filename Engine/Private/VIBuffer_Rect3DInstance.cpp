#include "VIBuffer_Rect3DInstance.h"
#define MAX_INSTANCE_COUNT 10000

D3DVERTEXELEMENT9 vertexDeclRect[] =
{
	// 정점 버퍼 (Stream 0)
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // vPosition
	{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, // vTexcoord

	// 인스턴스 버퍼 (Stream 1)
	{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 }, // vInstancePos (인스턴스 위치)

	D3DDECL_END()
};

CVIBuffer_Rect3DInstance::CVIBuffer_Rect3DInstance(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer { pGraphic_Device }

{
}

CVIBuffer_Rect3DInstance::CVIBuffer_Rect3DInstance(const CVIBuffer_Rect3DInstance& Prototype)
	: CVIBuffer(Prototype)
	, m_pInstanceVB{ Prototype.m_pInstanceVB }
	, m_iNumInstances{ Prototype.m_iNumInstances }
	, m_iInstanceStride{ Prototype.m_iInstanceStride }
	, pVertexDecl{ Prototype.pVertexDecl }
{
}

HRESULT CVIBuffer_Rect3DInstance::Initialize_Prototype()
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

	/* 인스턴싱 버퍼 생성*/
	if (FAILED(Create_InstanceBuffer()))
		return E_FAIL;

	HRESULT hr = m_pGraphic_Device->CreateVertexDeclaration(vertexDeclRect, &pVertexDecl);
	if (FAILED(hr) || pVertexDecl == nullptr)
	{
		MessageBox(0, L"Failed to create Vertex Declaration!", L"Error", MB_OK);
	}

	return S_OK;
}

HRESULT CVIBuffer_Rect3DInstance::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Rect3DInstance::Create_InstanceBuffer()
{
	m_iNumInstances = MAX_INSTANCE_COUNT;
	m_iInstanceStride = sizeof(D3DXVECTOR3);

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(
		m_iNumInstances * m_iInstanceStride,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&m_pInstanceVB,
		0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Rect3DInstance::Update_InstanceBuffer(const std::vector<D3DXVECTOR3>& positions)
{
	if (!m_pInstanceVB || positions.empty())
		return E_FAIL;

	// 현재 들어온 인스턴스 개수로 업데이트
	m_iNumInstances = static_cast<UINT>(positions.size());

	// 인스턴스 데이터가 담길 버퍼를 D3DXMATRIX로 변경
	D3DXMATRIX* pInstanceData = nullptr;
	if (FAILED(m_pInstanceVB->Lock(0, 0, (void**)&pInstanceData, D3DLOCK_DISCARD)))
		return E_FAIL;

	// 현재 들어온 인스턴스 개수만큼 복사
	memcpy(pInstanceData, positions.data(), m_iNumInstances * sizeof(D3DXVECTOR3));

	m_pInstanceVB->Unlock();
	return S_OK;
}

HRESULT CVIBuffer_Rect3DInstance::Render()
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
	m_pGraphic_Device->SetStreamSourceFreq(1, 1);

	return S_OK;
}


HRESULT CVIBuffer_Rect3DInstance::Bind_Buffers()
{
	if (!m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetVertexDeclaration(pVertexDecl);

	// 정점 버퍼 바인딩 (기본 버퍼)
	m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);
	m_pGraphic_Device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_iNumInstances);

	// 인스턴스 버퍼 바인딩 (위치 데이터)
	m_pGraphic_Device->SetStreamSource(1, m_pInstanceVB, 0, sizeof(D3DXVECTOR3));
	m_pGraphic_Device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	// 인덱스 버퍼 설정
	m_pGraphic_Device->SetIndices(m_pIB);


	return S_OK;
}

CVIBuffer_Rect3DInstance* CVIBuffer_Rect3DInstance::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Rect3DInstance* pInstance = new CVIBuffer_Rect3DInstance(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect3DInstance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Rect3DInstance::Clone(void* pArg)
{
	CVIBuffer_Rect3DInstance* pInstance = new CVIBuffer_Rect3DInstance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Rect3DInstance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect3DInstance::Free()
{
	__super::Free();
	Safe_Release(m_pInstanceVB);
	Safe_Release(pVertexDecl);

}
