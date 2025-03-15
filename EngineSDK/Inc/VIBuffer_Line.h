#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Line final : public CVIBuffer
{
private:
	CVIBuffer_Line(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Line(const CVIBuffer_Line& Prototype);
	virtual ~CVIBuffer_Line() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;


public:
	static CVIBuffer_Line* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END