#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect3D final : public CVIBuffer
{
private:
	CVIBuffer_Rect3D(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Rect3D(const CVIBuffer_Rect3D& Prototype);
	virtual ~CVIBuffer_Rect3D() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;


public:
	static CVIBuffer_Rect3D* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END