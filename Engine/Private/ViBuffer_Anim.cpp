#include "ViBuffer_Anim.h"
#include "Shader.h"

CVIBuffer_Anim::CVIBuffer_Anim(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Anim::CVIBuffer_Anim(const CVIBuffer_Anim& Prototype)
	: CVIBuffer(Prototype),
	pVertexDecl(Prototype.pVertexDecl)
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

HRESULT CVIBuffer_Anim::Bind_WorldMatrix(CShader* pShader)
{
	D3DXMATRIX viewMatrix, projMatrix;
	// 디바이스에서 뷰 행렬 가져오기
	if (FAILED(m_pGraphic_Device->GetTransform(D3DTS_VIEW, &viewMatrix)))
		return E_FAIL;

	// 디바이스에서 투영 행렬 가져오기
	if (FAILED(m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &projMatrix)))
		return E_FAIL;

	if (FAILED(pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(pShader->Bind_Matrix("g_ViewMatrix", &viewMatrix)))
		return E_FAIL;
	if (FAILED(pShader->Bind_Matrix("g_ProjMatrix", &projMatrix)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Anim::SetMatrix(const D3DXMATRIX& mat)
{
	m_WorldMatrix = mat;
}

HRESULT CVIBuffer_Anim::Render()
{
	m_pGraphic_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iNumVertices, 0, m_iNumPritimive);
	m_pGraphic_Device->SetStreamSourceFreq(0, 1);
	return S_OK;
}

HRESULT CVIBuffer_Anim::Bind_Buffers()
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

CComponent* CVIBuffer_Anim::Clone(void* pArg)
{
	return nullptr;
}

void CVIBuffer_Anim::Free()
{
	__super::Free();
	Safe_Release(pVertexDecl);
}
