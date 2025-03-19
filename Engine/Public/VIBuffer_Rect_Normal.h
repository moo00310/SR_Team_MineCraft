#pragma once

#include "VIBuffer.h"

BEGIN(Engine)


class ENGINE_DLL CVIBuffer_Rect_Normal final : public CVIBuffer
{
private:
	CVIBuffer_Rect_Normal(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Rect_Normal(const CVIBuffer_Rect_Normal& Prototype);
	virtual ~CVIBuffer_Rect_Normal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	
private:
	HRESULT DrawRect();
	

public:
	static CVIBuffer_Rect_Normal* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END