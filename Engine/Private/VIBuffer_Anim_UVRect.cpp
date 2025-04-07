#include "VIBuffer_Anim_UVRect.h"

D3DVERTEXELEMENT9 vertex_rect[] =
{
	// 정점 버퍼 (Stream 0)
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // vPosition
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   // vNormal
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, // vTexcoord
	D3DDECL_END()
};

CVIBuffer_Anim_UVRect::CVIBuffer_Anim_UVRect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer_Anim{ pGraphic_Device }
{
}

CVIBuffer_Anim_UVRect::CVIBuffer_Anim_UVRect(const CVIBuffer_Anim_UVRect& Prototype)
	: CVIBuffer_Anim(Prototype)
{
}

HRESULT CVIBuffer_Anim_UVRect::Initialize_Prototype(RECT_INFO& tInfo)
{

	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPritimive = 2;

	m_iIndexStride = 2;
	m_iNumIndices = 6;
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(DrawRect(tInfo)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Anim_UVRect::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Anim_UVRect::DrawRect(RECT_INFO& tInfo)
{
#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORTEX* pVertices = { nullptr };

	_float2 startPixelpos = tInfo.Pixelpos;
	_float2 Imagesize = tInfo.Imagesize;
	_float2 fXYSize = tInfo.XY_SIze;


	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vNormal = _float3(-1.0f, 1.0f, -1.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vNormal = _float3(1.0f, 1.0f, -1.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vNormal = _float3(1.0f, -1.0f, -1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vNormal = _float3(-1.0f, -1.0f, -1.f);

	/// 텍스 쿠드
	pVertices[0].vTexcoord = _float2((startPixelpos.x) / Imagesize.x,				(startPixelpos.y) / Imagesize.y);  //0,0
	pVertices[1].vTexcoord = _float2((startPixelpos.x + fXYSize.x) / Imagesize.x,	(startPixelpos.y) / Imagesize.y); // 1,0
	pVertices[2].vTexcoord = _float2((startPixelpos.x + fXYSize.x) / Imagesize.x,	(startPixelpos.y + fXYSize.y) / Imagesize.y); //1,1
	pVertices[3].vTexcoord = _float2((startPixelpos.x )/ Imagesize.x,				(startPixelpos.y + fXYSize.y) / Imagesize.y ); // 0,1

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

	HRESULT hr = m_pGraphic_Device->CreateVertexDeclaration(vertex_rect, &pVertexDecl);
	if (FAILED(hr) || pVertexDecl == nullptr)
	{
		MessageBox(0, L"Failed to create Vertex Declaration!", L"Error", MB_OK);
	}


	return S_OK;
}

CVIBuffer_Anim_UVRect* CVIBuffer_Anim_UVRect::Create(LPDIRECT3DDEVICE9 pGraphic_Device, RECT_INFO tInfo)
{
	CVIBuffer_Anim_UVRect* pInstance = new CVIBuffer_Anim_UVRect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(tInfo)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Anim_UVRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Anim_UVRect::Clone(void* pArg)
{
	CVIBuffer_Anim_UVRect* pInstance = new CVIBuffer_Anim_UVRect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Anim_UVRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Anim_UVRect::Free()
{
	__super::Free();

}
