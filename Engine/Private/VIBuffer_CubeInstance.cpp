#include "VIBuffer_CubeInstance.h"
#define MAX_INSTANCE_COUNT 10000

D3DVERTEXELEMENT9 vertexCubeInstance[] =
{
    // 정점 버퍼 (Stream 0)
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }, // vPosition
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   // vNormal
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, // vTexcoord

    // 인스턴스 버퍼 (Stream 1)
    { 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 }, // vInstancePos (인스턴스 위치)
    { 1, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 }, // vInstanceBright (인스턴스 밝기)

    D3DDECL_END()
};

CVIBuffer_CubeInstance::CVIBuffer_CubeInstance(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_CubeInstance::CVIBuffer_CubeInstance(const CVIBuffer_CubeInstance& Prototype)
    : CVIBuffer (Prototype)
    ,m_pInstanceVB{ Prototype.m_pInstanceVB }
    , m_iNumInstances {Prototype.m_iNumInstances}
    , m_iInstanceStride{Prototype.m_iInstanceStride }
    , pVertexDecl{Prototype.pVertexDecl }

{
    D3DXMatrixIdentity(&m_WorldMatrix);
}

HRESULT CVIBuffer_CubeInstance::Initialize_Prototype(CUBE& tInfo)
{
    m_iNumVertices = 24;
    m_iVertexStride = sizeof(VTXNORTEX);
    m_iFVF = D3DFVF_XYZ | D3DFVF_NORMAL |D3DFVF_TEX1;
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


    VTXNORTEX* pVertices = { nullptr };
    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);


#pragma region 버택스 점찍기

    // front 앞 면
    pVertices[0].vPosition = VertInex[0];
    pVertices[1].vPosition = VertInex[1];
    pVertices[2].vPosition = VertInex[2];
    pVertices[3].vPosition = VertInex[3];

    // Right 오른쪽 면
    pVertices[4].vPosition = VertInex[3];
    pVertices[5].vPosition = VertInex[2];
    pVertices[6].vPosition = VertInex[6];
    pVertices[7].vPosition = VertInex[7];

    // back 뒷면
    pVertices[8].vPosition = VertInex[7];
    pVertices[9].vPosition = VertInex[6];
    pVertices[10].vPosition = VertInex[5];
    pVertices[11].vPosition = VertInex[4];

    // left 왼쪽면
    pVertices[12].vPosition = VertInex[4];
    pVertices[13].vPosition = VertInex[5];
    pVertices[14].vPosition = VertInex[1];
    pVertices[15].vPosition = VertInex[0];

    // Up 윗면
    pVertices[16].vPosition = VertInex[4];
    pVertices[17].vPosition = VertInex[0];
    pVertices[18].vPosition = VertInex[3];
    pVertices[19].vPosition = VertInex[7];

    // Down 아랫면
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

#pragma region UV 매핑
    
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

#pragma region Normal 매핑
    _float3		vSourDir, vDestDir, vNormal;

    for (int i = 0; i < 34; i++)
    {
        vSourDir = pVertices[pIndices[i+1]].vPosition - pVertices[pIndices[i]].vPosition;
        vDestDir = pVertices[pIndices[i+2]].vPosition - pVertices[pIndices[i+1]].vPosition;

        D3DXVec3Normalize(&vNormal, D3DXVec3Cross(&vNormal, &vSourDir, &vDestDir));

        pVertices[pIndices[i]].vNormal += vNormal;
        pVertices[pIndices[i+1]].vNormal += vNormal;
        pVertices[pIndices[i+2]].vNormal += vNormal;
    }
  
#pragma endregion

    m_pVB->Unlock();
    m_pIB->Unlock();

    /* 인스턴싱 버퍼 생성*/
    if (FAILED(Create_InstanceBuffer()))
        return E_FAIL;


     HRESULT hr = m_pGraphic_Device->CreateVertexDeclaration(vertexCubeInstance, &pVertexDecl);
    if (FAILED(hr) || pVertexDecl == nullptr)
    {
        MessageBox(0, L"Failed to create Vertex Declaration!", L"Error", MB_OK);
    }
    
    return S_OK;
}

HRESULT CVIBuffer_CubeInstance::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CVIBuffer_CubeInstance::Bind_WorldMatrix()
{
    m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
    return S_OK;
}

void CVIBuffer_CubeInstance::SetMatrix(const D3DXMATRIX& mat)
{
    m_WorldMatrix = mat;
}

HRESULT CVIBuffer_CubeInstance::Render()
{

    HRESULT hr = m_pGraphic_Device->DrawIndexedPrimitive(
        D3DPT_TRIANGLELIST,  // 삼각형 리스트
        0,                   // 정점 시작 인덱스
        0,                   // 최소 정점 인덱스
        m_iNumVertices,      // 총 정점 개수
        0,                   // 인덱스 버퍼 오프셋
        m_iNumPritimive      // 삼각형 개수
    );

    m_pGraphic_Device->SetStreamSourceFreq(0, 1);
    m_pGraphic_Device->SetStreamSourceFreq(1, 1);

    return hr;
}

HRESULT CVIBuffer_CubeInstance::Create_InstanceBuffer()
{
    m_iNumInstances = MAX_INSTANCE_COUNT;
    m_iInstanceStride = sizeof(INFOINS);

    if (FAILED(m_pGraphic_Device->CreateVertexBuffer(
        m_iNumInstances * m_iInstanceStride, 
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
        0,
        D3DPOOL_DEFAULT,
        &m_pInstanceVB,
        0)))
        return E_FAIL;

    return S_OK;
}


HRESULT CVIBuffer_CubeInstance::Bind_Buffers()
{
    if (!m_pGraphic_Device)
        return E_FAIL;

    m_pGraphic_Device->SetVertexDeclaration(pVertexDecl);

    // 정점 버퍼 바인딩 (기본 버퍼)
    m_pGraphic_Device->SetStreamSource(0, m_pVB, 0, m_iVertexStride);
    m_pGraphic_Device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_iNumInstances);

    // 인스턴스 버퍼 바인딩 (위치 데이터)
    m_pGraphic_Device->SetStreamSource(1, m_pInstanceVB, 0, sizeof(INFOINS));
    m_pGraphic_Device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

    // 인덱스 버퍼 설정
    m_pGraphic_Device->SetIndices(m_pIB);

    
    return S_OK;
}

HRESULT CVIBuffer_CubeInstance::Update_InstanceBuffer(vector<D3DXVECTOR3>& positions, vector<_float>& brights)
{
    if (!m_pInstanceVB || positions.empty() || brights.empty())
        return E_FAIL;

    // 현재 들어온 인스턴스 개수로 업데이트
    m_iNumInstances = static_cast<UINT>(positions.size());

    // 인스턴스 데이터가 담길 버퍼를 D3DXMATRIX로 변경
    INFOINS* pInstanceData = nullptr;
    if (FAILED(m_pInstanceVB->Lock(0, 0, (void**)&pInstanceData, D3DLOCK_DISCARD)))
        return E_FAIL;

    for (_uint i = 0; i < m_iNumInstances; ++i) {
        pInstanceData[i].vPosition = positions[i];
        pInstanceData[i].vBright = brights[i];
    }

    m_pInstanceVB->Unlock();
    return S_OK;
}

HRESULT CVIBuffer_CubeInstance::Update_InstanceBuffer(vector<D3DXVECTOR3>& positions, _float _bright)
{
    if (!m_pInstanceVB || positions.empty())
        return E_FAIL;

    // 현재 들어온 인스턴스 개수로 업데이트
    m_iNumInstances = static_cast<UINT>(positions.size());

    INFOINS* pInstanceData = nullptr;
    if (FAILED(m_pInstanceVB->Lock(0, 0, (void**)&pInstanceData, D3DLOCK_DISCARD)))
        return E_FAIL;

    for (_uint i = 0; i < m_iNumInstances; ++i) {
        pInstanceData[i].vPosition = positions[i];
        pInstanceData[i].vBright = _bright;
    }

    m_pInstanceVB->Unlock();
    return S_OK;
}

CVIBuffer_CubeInstance* CVIBuffer_CubeInstance::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CUBE tInfo)
{
    CVIBuffer_CubeInstance* pInstance = new CVIBuffer_CubeInstance(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(tInfo)))
    {
        MSG_BOX("Failed to Created : CVIBuffer_CubeInstance");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_CubeInstance::Clone(void* pArg)
{
    CVIBuffer_CubeInstance* pInstance = new CVIBuffer_CubeInstance(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CVIBuffer_CubeInstance");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CVIBuffer_CubeInstance::Free()
{
	__super::Free();

    Safe_Release(m_pInstanceVB);
    Safe_Release(pVertexDecl);
}
