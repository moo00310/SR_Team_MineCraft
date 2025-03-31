#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrustumCulling_Manager final : public CBase
{
private:
    CFrustumCulling_Manager(LPDIRECT3DDEVICE9 pGraphicDevice);
    virtual ~CFrustumCulling_Manager() = default;

public:
    HRESULT	Initilize(LPDIRECT3DDEVICE9 pGraphicDevice);
    HRESULT Update();
    _bool   Is_In_Frustum(_float3 vPos, _float fRadius);

private:
    LPDIRECT3DDEVICE9 m_pGraphicDevice{ nullptr };
    _float3 m_WorldPoint[8]{};
    D3DXPLANE m_FrustumPlane[6]{};

public:
    static CFrustumCulling_Manager* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
    virtual void Free() override;
};

END

