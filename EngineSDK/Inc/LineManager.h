#pragma once
#include "Base.h"

#define MAX_LINES 30  // 최대 라인 개수

class CLineManager : public CBase
{
public:
    typedef struct tagLine
    {
        _float3			vRayOrigin{};
        _float3			vRayDir{};
        _float			fMaxDistance{};
        _bool			isHit{ false };
    }LINE;

private:
    CLineManager(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual ~CLineManager() = default;

private:
    LPDIRECT3DDEVICE9 m_pGraphic_Device;

private:
    vector<LINE> m_Lines;
    int m_CurrIndex = 0;

public:

    void Add_Line(const _float3& origin, const _float3& dir, _float maxDist, _bool isHit);
    void Draw_Line(LINE& tLine);
    void Render_Lines();

public:
    static CLineManager* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual void Free() override;
};

