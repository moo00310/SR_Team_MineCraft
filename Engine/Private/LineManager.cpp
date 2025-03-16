#include "LineManager.h"

CLineManager::CLineManager(LPDIRECT3DDEVICE9 pGraphic_Device)
	:m_pGraphic_Device(pGraphic_Device)
{
	Safe_AddRef(m_pGraphic_Device);
	m_Lines.resize(MAX_LINES);  // 미리 공간 확보
}

void CLineManager::Add_Line(const _float3& origin, const _float3& dir, _float maxDist, _bool isHit)
{
    LINE& line = m_Lines[m_CurrIndex];
    line.vRayOrigin = origin;
    line.vRayDir = dir;
    line.fMaxDistance = maxDist;
    line.isHit = isHit;  // 초기화

    m_CurrIndex = (m_CurrIndex + 1) % MAX_LINES;  // 인덱스 순환
}

void CLineManager::Draw_Line(LINE& tLine)
{
    // 기존 상태 저장
    DWORD prevColorOp, prevColorArg1;
    m_pGraphic_Device->GetTextureStageState(0, D3DTSS_COLOROP, &prevColorOp);
    m_pGraphic_Device->GetTextureStageState(0, D3DTSS_COLORARG1, &prevColorArg1);

    if (tLine.isHit)
    {
        m_pGraphic_Device->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 0, 0)); // 빨간색
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
    }

    D3DXVECTOR3 end = tLine.vRayOrigin + tLine.vRayDir * tLine.fMaxDistance;

    VTXPOSCOL line[] =
    {
        { tLine.vRayOrigin, D3DCOLOR_XRGB(255, 0, 0) },  // 빨간색 시작점
        { end,   D3DCOLOR_XRGB(255, 255, 0) } // 노란색 끝점
    };

    m_pGraphic_Device->DrawPrimitiveUP(D3DPT_LINELIST, 1, line, sizeof(VTXPOSCOL));

    // 상태 복구
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLOROP, prevColorOp);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_COLORARG1, prevColorArg1);

    // 구조체 초기화
    ZeroMemory(&tLine, sizeof(tLine));

}

void CLineManager::Render_Lines()
{

    _float4x4 identityMatrix;
    D3DXMatrixIdentity(&identityMatrix);
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &identityMatrix);
    m_pGraphic_Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

    for (auto& line : m_Lines)
    {
        if (line.fMaxDistance > 0.0f)
        {
            Draw_Line(line);
        }
    }
}

CLineManager* CLineManager::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return new CLineManager(pGraphic_Device);
}

void CLineManager::Free()
{
	__super::Free();

	Safe_Release(m_pGraphic_Device);
}
