#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cube_Only : public CVIBuffer
{
protected:
	CVIBuffer_Cube_Only(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Cube_Only(const CVIBuffer_Cube_Only& Prototype);
	virtual ~CVIBuffer_Cube_Only() = default;

public:
	virtual HRESULT Initialize_Prototype(CUBE_ONLY& tInfo);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Cube_Only* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CUBE_ONLY tInfo);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

