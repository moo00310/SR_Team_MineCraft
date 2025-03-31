#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect3DInstance final : public CVIBuffer
{
private:
	CVIBuffer_Rect3DInstance(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Rect3DInstance(const CVIBuffer_Rect3DInstance& Prototype);
	virtual ~CVIBuffer_Rect3DInstance() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Create_InstanceBuffer();
	HRESULT Update_InstanceBuffer(vector<D3DXVECTOR3>& positions, vector<_float>& brights);
	HRESULT Update_InstanceBuffer(vector<D3DXVECTOR3>& positions, _float bright);
	HRESULT Render();
	HRESULT Bind_Buffers();
private:
	IDirect3DVertexBuffer9* m_pInstanceVB = nullptr;
	UINT m_iNumInstances = 0;
	UINT m_iInstanceStride = 0;
	IDirect3DVertexDeclaration9* pVertexDecl = nullptr;
public:
	static CVIBuffer_Rect3DInstance* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END