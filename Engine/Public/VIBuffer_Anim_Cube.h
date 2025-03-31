#pragma once

#include "VIBuffer_Anim.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Anim_Cube final : public CVIBuffer_Anim
{
protected:
	CVIBuffer_Anim_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Anim_Cube(const CVIBuffer_Anim_Cube& Prototype);
	virtual ~CVIBuffer_Anim_Cube() = default;

public:
	virtual HRESULT Initialize_Prototype(CUBE& tInfo);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Anim_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CUBE tInfo);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

