#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectShader final : public CVIBuffer
{
private:
	CVIBuffer_RectShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_RectShader(const CVIBuffer_RectShader& Prototype);
	virtual ~CVIBuffer_RectShader() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual HRESULT Bind_Buffers() override;
	virtual HRESULT Render() override;
private:
	IDirect3DVertexDeclaration9* pVertexDecl = nullptr;
public:
	static CVIBuffer_RectShader* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END