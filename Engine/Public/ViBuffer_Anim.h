#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Anim abstract : public CVIBuffer
{
protected:
	CVIBuffer_Anim(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Anim(const CVIBuffer_Anim& Prototype);
	virtual ~CVIBuffer_Anim() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Bind_WorldMatrix();
	void SetMatrix(const D3DXMATRIX& mat);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() =0;

protected:
	_float4x4				m_WorldMatrix = {};

};

END