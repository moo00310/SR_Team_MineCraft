#include "VIBuffer_Anim_Cube.h"

D3DVERTEXELEMENT9 vertexCube_cube[] =
{
    // ���� ���� (Stream 0)
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // vPosition
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   // vNormal
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, // vTexcoord
    D3DDECL_END()
};

CVIBuffer_Anim_Cube::CVIBuffer_Anim_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer_Anim { pGraphic_Device }
{
}

CVIBuffer_Anim_Cube::CVIBuffer_Anim_Cube(const CVIBuffer_Anim_Cube& Prototype) :
    CVIBuffer_Anim (Prototype)
{
    D3DXMatrixIdentity(&m_WorldMatrix);
}

HRESULT CVIBuffer_Anim_Cube::Initialize_Prototype(CUBE& tInfo)
{
    m_iNumVertices = 24;
    m_iVertexStride = sizeof(VTXNORTEX);
    m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
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
        {_float3(fBoxSize.x / 16 * 0.5f,   fBoxSize.y / 16 * -0.5f,  fBoxSize.z / 16 * -0.5f)},
        {_float3(fBoxSize.x / 16 * 0.5f,    fBoxSize.y / 16 * 0.5f,   fBoxSize.z / 16 * -0.5f)},
        {_float3(fBoxSize.x / 16 * -0.5f,   fBoxSize.y / 16 * 0.5f,   fBoxSize.z / 16 * 0.5f)},
        {_float3(fBoxSize.x / 16 * -0.5f,   fBoxSize.y / 16 * -0.5f,  fBoxSize.z / 16 * 0.5f)},
        {_float3(fBoxSize.x / 16 * 0.5f,    fBoxSize.y / 16 * -0.5f,  fBoxSize.z / 16 * 0.5f)},
        {_float3(fBoxSize.x / 16 * 0.5f,    fBoxSize.y / 16 * 0.5f,   fBoxSize.z / 16 * 0.5f)},
    };


    VTXNORTEX* pVertices = { nullptr };
    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);


#pragma region ���ý� �����

    // front �� ��
    pVertices[0].vPosition = VertInex[0];
    pVertices[1].vPosition = VertInex[1];
    pVertices[2].vPosition = VertInex[2];
    pVertices[3].vPosition = VertInex[3];

    // back �޸�
    pVertices[4].vPosition = VertInex[7];
    pVertices[5].vPosition = VertInex[6];
    pVertices[6].vPosition = VertInex[5];
    pVertices[7].vPosition = VertInex[4];

    // Right ������ ��
    pVertices[8].vPosition = VertInex[3];
    pVertices[9].vPosition = VertInex[2];
    pVertices[10].vPosition = VertInex[6];
    pVertices[11].vPosition = VertInex[7];

    // left ���ʸ�
    pVertices[12].vPosition = VertInex[4];
    pVertices[13].vPosition = VertInex[5];
    pVertices[14].vPosition = VertInex[1];
    pVertices[15].vPosition = VertInex[0];

    // Up ����
    pVertices[16].vPosition = VertInex[4];
    pVertices[17].vPosition = VertInex[0];
    pVertices[18].vPosition = VertInex[3];
    pVertices[19].vPosition = VertInex[7];

    // Down �Ʒ���
    pVertices[20].vPosition = VertInex[1];
    pVertices[21].vPosition = VertInex[5];
    pVertices[22].vPosition = VertInex[6];
    pVertices[23].vPosition = VertInex[2];

#pragma endregion

    _float2 UvPexel[6] =
    {
        // front,back
        {_float2(startPixelpos.x + fBoxSize.z * 2 + fBoxSize.x,  startPixelpos.y + fBoxSize.z)},
        {_float2(startPixelpos.x + fBoxSize.z,                   startPixelpos.y + fBoxSize.z)},

        // right, left
        {_float2(startPixelpos.x,                                startPixelpos.y + fBoxSize.z)},
        {_float2(startPixelpos.x + fBoxSize.z + fBoxSize.x,      startPixelpos.y + fBoxSize.z)},

        // top, bottom
        {_float2(startPixelpos.x + fBoxSize.z,                   startPixelpos.y)},
        {_float2(startPixelpos.x + fBoxSize.z + fBoxSize.x,      startPixelpos.y)}
    };

#pragma region UV ����

    _uint		iUvIndices = { 0 };

    for (int i = 0; i < 2; i++)
    {
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x) / Imagesize.x,               (UvPexel[i].y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x) / Imagesize.x,               (UvPexel[i].y + fBoxSize.y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x + fBoxSize.x) / Imagesize.x,  (UvPexel[i].y + fBoxSize.y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x + fBoxSize.x) / Imagesize.x,  (UvPexel[i].y) / Imagesize.y);
    }

    for (int i = 2; i < 4; i++)
    {
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x) / Imagesize.x,                (UvPexel[i].y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x) / Imagesize.x,                (UvPexel[i].y + fBoxSize.y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x + fBoxSize.z) / Imagesize.x,   (UvPexel[i].y + fBoxSize.y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x + fBoxSize.z) / Imagesize.x,   (UvPexel[i].y) / Imagesize.y);
    }

    for (int i = 4; i < 6; i++)
    {
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x) / Imagesize.x, (UvPexel[i].y + fBoxSize.z) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x) / Imagesize.x, (UvPexel[i].y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x + fBoxSize.x) / Imagesize.x, (UvPexel[i].y) / Imagesize.y);
        pVertices[iUvIndices++].vTexcoord = _float2((UvPexel[i].x + fBoxSize.x) / Imagesize.x, (UvPexel[i].y + fBoxSize.z) / Imagesize.y);
    }

#pragma endregion
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

#pragma region Normal ����
    _float3		vSourDir, vDestDir, vNormal;

    for (int i = 0; i < 34; i++)
    {
        vSourDir = pVertices[pIndices[i + 1]].vPosition - pVertices[pIndices[i]].vPosition;
        vDestDir = pVertices[pIndices[i + 2]].vPosition - pVertices[pIndices[i + 1]].vPosition;

        D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir));

        pVertices[pIndices[i]].vNormal += vNormal;
        pVertices[pIndices[i + 1]].vNormal += vNormal;
        pVertices[pIndices[i + 2]].vNormal += vNormal;
    }

#pragma endregion

    m_pVB->Unlock();
    m_pIB->Unlock();

    HRESULT hr = m_pGraphic_Device->CreateVertexDeclaration(vertexCube_cube, &pVertexDecl);
    if (FAILED(hr) || pVertexDecl == nullptr)
    {
        MessageBox(0, L"Failed to create Vertex Declaration!", L"Error", MB_OK);
    }


    return S_OK;

}

HRESULT CVIBuffer_Anim_Cube::Initialize(void* pArg)
{
    return S_OK;
}


CVIBuffer_Anim_Cube* CVIBuffer_Anim_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CUBE tInfo)
{
    CVIBuffer_Anim_Cube* pInstance = new CVIBuffer_Anim_Cube(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(tInfo)))
    {
        MSG_BOX("Failed to Created : CVIBuffer_Anim_Cube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Anim_Cube::Clone(void* pArg)
{
    CVIBuffer_Anim_Cube* pInstance = new CVIBuffer_Anim_Cube(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CVIBuffer_Anim_Cube");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CVIBuffer_Anim_Cube::Free()
{
    __super::Free();
}
