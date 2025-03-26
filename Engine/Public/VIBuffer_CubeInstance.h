#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_CubeInstance final : public CVIBuffer
{
public:
	struct InstanceData
	{
		D3DXVECTOR3 vPosition;  // 오브젝트의 위치
	};
private:
	CVIBuffer_CubeInstance(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_CubeInstance(const CVIBuffer_CubeInstance& Prototype);
	virtual ~CVIBuffer_CubeInstance() = default;

public:
	virtual HRESULT Initialize_Prototype(CUBE& tInfo);
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Bind_WorldMatrix();
	HRESULT Create_InstanceBuffer();
	virtual HRESULT Bind_Buffers();
	HRESULT Update_InstanceBuffer(const std::vector<D3DXVECTOR3>& positions);
	void SetMatrix(const D3DXMATRIX& mat);
	virtual HRESULT Render();
private:
	IDirect3DVertexBuffer9* m_pInstanceVB = nullptr;
	UINT m_iNumInstances = 0;
	UINT m_iInstanceStride = 0;

	IDirect3DVertexDeclaration9* pVertexDecl = nullptr;
public:
	static CVIBuffer_CubeInstance* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CUBE tInfo);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float4x4				m_WorldMatrix = {};
};

END
