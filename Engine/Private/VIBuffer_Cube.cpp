#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& Prototype):
    CVIBuffer (Prototype)
{
    D3DXMatrixIdentity(&m_WorldMatrix);
}

HRESULT CVIBuffer_Cube::Initialize_Prototype(CUBE& tInfo)
{
    m_iNumVertices = 24;
    m_iVertexStride = sizeof(VTXPOSTEX);
    m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;
    m_iNumPritimive = 12;

    m_iNumIndices = m_iNumPritimive * 3;
    m_iIndexStride = 4;
    m_eIndexFormat = D3DFMT_INDEX32;

    _float2 startPixelpos = tInfo.Pixelpos;
    _float2 Imagesize = tInfo.Imagesize;
    _float3 fBoxSize = tInfo.BoxSIze;

    /*VB*/
    if (FAILED(__super::Create_VertexBuffer()))
        return E_FAIL;

    _float3 VertInex[8] =
    {
        {_float3(fBoxSize.x / 16 * -0.5f,   fBoxSize.y / 16 * +0.5f,  fBoxSize.z / 16 * -0.5f)},
        {_float3(fBoxSize.x / 16 * -0.5f,   fBoxSize.y / 16 * -0.5f,  fBoxSize.z / 16 * -0.5f)},
        {_float3(fBoxSize.x / 16 *  0.5f,   fBoxSize.y / 16 * -0.5f,  fBoxSize.z / 16 * -0.5f)},
        {_float3(fBoxSize.x / 16 * 0.5f,    fBoxSize.y / 16 * 0.5f,   fBoxSize.z / 16 * -0.5f)},
        {_float3(fBoxSize.x / 16 * -0.5f,   fBoxSize.y / 16 * 0.5f,   fBoxSize.z / 16 * 0.5f)},
        {_float3(fBoxSize.x / 16 * -0.5f,   fBoxSize.y / 16 * -0.5f,  fBoxSize.z / 16 * 0.5f)},
        {_float3(fBoxSize.x / 16 * 0.5f,    fBoxSize.y / 16 * -0.5f,  fBoxSize.z / 16 * 0.5f)},
        {_float3(fBoxSize.x / 16 * 0.5f,    fBoxSize.y / 16 * 0.5f,   fBoxSize.z / 16 * 0.5f)},
    };


    VTXPOSTEX* pVertices = { nullptr };
    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);


#pragma region ¹öÅÃ½º Á¡Âï±â
    // front ¾Õ ¸é
    pVertices[0].vPosition = VertInex[0];
    pVertices[1].vPosition = VertInex[1];
    pVertices[2].vPosition = VertInex[2];
    pVertices[3].vPosition = VertInex[3];

    // Right ¿À¸¥ÂÊ ¸é
    pVertices[4].vPosition = VertInex[3];
    pVertices[5].vPosition = VertInex[2];
    pVertices[6].vPosition = VertInex[6];
    pVertices[7].vPosition = VertInex[7];

    // back µÞ¸é
    pVertices[8].vPosition = VertInex[7];
    pVertices[9].vPosition = VertInex[6];
    pVertices[10].vPosition = VertInex[5];
    pVertices[11].vPosition = VertInex[4];

    // left ¿ÞÂÊ¸é
    pVertices[12].vPosition = VertInex[4];
    pVertices[13].vPosition = VertInex[5];
    pVertices[14].vPosition = VertInex[1];
    pVertices[15].vPosition = VertInex[0];

    // Up À­¸é
    pVertices[16].vPosition = VertInex[4];
    pVertices[17].vPosition = VertInex[0];
    pVertices[18].vPosition = VertInex[3];
    pVertices[19].vPosition = VertInex[7];

    // Down ¾Æ·§¸é
    pVertices[20].vPosition = VertInex[1];
    pVertices[21].vPosition = VertInex[5];
    pVertices[22].vPosition = VertInex[6];
    pVertices[23].vPosition = VertInex[2];

#pragma endregion

    _float2 UvPexel[6] =
    {
        {_float2(startPixelpos.x + fBoxSize.z * 2 + fBoxSize.x,  startPixelpos.y + fBoxSize.z)},
        {_float2(startPixelpos.x,                                startPixelpos.y + fBoxSize.z)},
        {_float2(startPixelpos.x + fBoxSize.z,                   startPixelpos.y + fBoxSize.z)},
        {_float2(startPixelpos.x + fBoxSize.z + fBoxSize.x,      startPixelpos.y + fBoxSize.z)},
        {_float2(startPixelpos.x + fBoxSize.z,                   startPixelpos.y)},
        {_float2(startPixelpos.x + fBoxSize.z + fBoxSize.x,      startPixelpos.y)} 
    };

#pragma region UV ¸ÅÇÎ
    
    _uint		iUvIndices = { 0 };
    // front, Right, back, left
    for (int i = 0; i < 4; i++)
    {
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x) / Imagesize.x,              (UvPexel[i].y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x) / Imagesize.x,              (UvPexel[i].y + fBoxSize.y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x + fBoxSize.x) / Imagesize.x, (UvPexel[i].y + fBoxSize.y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x + fBoxSize.x) / Imagesize.x, (UvPexel[i].y) / Imagesize.y);
    }
   
    for (int i = 4; i < 6; i++)
    {
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x) / Imagesize.x,               (UvPexel[i].y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x) / Imagesize.x,               (UvPexel[i].y + fBoxSize.z) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x + fBoxSize.x) / Imagesize.x, (UvPexel[i].y + fBoxSize.z) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x + fBoxSize.x) / Imagesize.x, (UvPexel[i].y) / Imagesize.y);
    }
#pragma endregion

    m_pVB->Unlock();

    /*IB*/
    if (FAILED(__super::Create_IndexBuffer()))
        return E_FAIL;

    _uint		iNumIndices = { 0 };
    _uint* pIndices = { nullptr };

    m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

    pIndices[iNumIndices++] = 0;  pIndices[iNumIndices++] = 3;  pIndices[iNumIndices++] = 2;
    pIndices[iNumIndices++] = 0;  pIndices[iNumIndices++] = 2;  pIndices[iNumIndices++] = 1;

    pIndices[iNumIndices++] = 4;  pIndices[iNumIndices++] = 7;  pIndices[iNumIndices++] = 6;
    pIndices[iNumIndices++] = 4;  pIndices[iNumIndices++] = 6;  pIndices[iNumIndices++] = 5;

    pIndices[iNumIndices++] = 8;  pIndices[iNumIndices++] = 11;  pIndices[iNumIndices++] = 10;
    pIndices[iNumIndices++] = 8;  pIndices[iNumIndices++] = 10;  pIndices[iNumIndices++] = 9;

    pIndices[iNumIndices++] = 12;  pIndices[iNumIndices++] = 15;  pIndices[iNumIndices++] = 14;
    pIndices[iNumIndices++] = 12;  pIndices[iNumIndices++] = 14;  pIndices[iNumIndices++] = 13;

    pIndices[iNumIndices++] = 16;  pIndices[iNumIndices++] = 19;  pIndices[iNumIndices++] = 18;
    pIndices[iNumIndices++] = 16;  pIndices[iNumIndices++] = 18;  pIndices[iNumIndices++] = 17;

    pIndices[iNumIndices++] = 20;  pIndices[iNumIndices++] = 23;  pIndices[iNumIndices++] = 22;
    pIndices[iNumIndices++] = 20;  pIndices[iNumIndices++] = 22;  pIndices[iNumIndices++] = 21;

    m_pIB->Unlock();

    return S_OK;

}

HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer_Cube::Bind_WorldMatrix()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
    return S_OK;
}

void CVIBuffer_Cube::SetMatrix(const D3DXMATRIX& mat)
{
    m_WorldMatrix = mat;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CUBE tInfo)
{
    CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(tInfo)))
    {
        MSG_BOX("Failed to Created : CVIBuffer_Cube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
    CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CVIBuffer_Cube");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CVIBuffer_Cube::Free()
{
	__super::Free();
}
