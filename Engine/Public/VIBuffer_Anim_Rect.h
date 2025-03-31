#pragma once

#include "VIBuffer_Anim.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Anim_Rect final : public CVIBuffer_Anim
{
protected:
	CVIBuffer_Anim_Rect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Anim_Rect(const CVIBuffer_Anim_Rect& Prototype);
	virtual ~CVIBuffer_Anim_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	HRESULT DrawRect();

public:
	static CVIBuffer_Anim_Rect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END