#pragma once

#include "VIBuffer_Anim.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Anim_UVRect final : public CVIBuffer_Anim
{
protected:
	CVIBuffer_Anim_UVRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Anim_UVRect(const CVIBuffer_Anim_UVRect& Prototype);

	virtual ~CVIBuffer_Anim_UVRect() = default;

public:
	HRESULT Initialize_Prototype(RECT_INFO& tInfo);
	virtual HRESULT Initialize(void* pArg) override;

private:
	HRESULT DrawRect(RECT_INFO& tInfo);

public:
	static 	CVIBuffer_Anim_UVRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device, RECT_INFO tInfo);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END