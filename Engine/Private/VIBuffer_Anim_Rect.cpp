#include "VIBuffer_Anim_Rect.h"

CVIBuffer_Anim_Rect::CVIBuffer_Anim_Rect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer_Anim{ pGraphic_Device }
{
}

CVIBuffer_Anim_Rect::CVIBuffer_Anim_Rect(const CVIBuffer_Anim_Rect& Prototype)
	: CVIBuffer_Anim(Prototype)
{
}

HRESULT CVIBuffer_Anim_Rect::Initialize_Prototype()
{

	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	m_iNumPritimive = 2;

	m_iIndexStride = 2;
	m_iNumIndices = 6;
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(DrawRect()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Anim_Rect::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Anim_Rect::DrawRect()
{
#pragma region VERTEX_BUFFER
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORTEX* pVertices = { nullptr };


	m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vNormal = _float3(-1.0f, 1.0f, -1.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vNormal = _float3(1.0f, 1.0f, -1.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vNormal = _float3(1.0f, -1.0f, -1.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vNormal = _float3(-1.0f, -1.0f, -1.f);
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

	return S_OK;
}

CVIBuffer_Anim_Rect* CVIBuffer_Anim_Rect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Anim_Rect* pInstance = new CVIBuffer_Anim_Rect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Anim_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Anim_Rect::Clone(void* pArg)
{
	CVIBuffer_Anim_Rect* pInstance = new CVIBuffer_Anim_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Anim_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Anim_Rect::Free()
{
	__super::Free();
}
