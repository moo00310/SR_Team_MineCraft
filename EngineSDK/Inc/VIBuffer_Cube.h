#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cube final : public CVIBuffer
{
private:
	CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Cube(const CVIBuffer_Cube& Prototype);
	virtual ~CVIBuffer_Cube() = default;

public:
	virtual HRESULT Initialize_Prototype(CUBE& tInfo);
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Bind_WorldMatrix();
	void SetMatrix(const D3DXMATRIX& mat);

public:
	static CVIBuffer_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CUBE tInfo);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float4x4				m_WorldMatrix = {};
};

END
