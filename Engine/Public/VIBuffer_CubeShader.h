#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_CubeShader final : public CVIBuffer
{
private:
	CVIBuffer_CubeShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_CubeShader(const CVIBuffer_CubeShader& Prototype);
	virtual ~CVIBuffer_CubeShader() = default;

public:
	virtual HRESULT Initialize_Prototype(CUBE& tInfo);
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Bind_WorldMatrix();
	void SetMatrix(const D3DXMATRIX& mat);

	virtual HRESULT Bind_Buffers() override;
private:
	IDirect3DVertexDeclaration9* pVertexDecl = nullptr;
public:
	static CVIBuffer_CubeShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CUBE tInfo);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float4x4				m_WorldMatrix = {};
};

END
