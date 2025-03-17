#include "VIBuffer_Cube_Only.h"

CVIBuffer_Cube_Only::CVIBuffer_Cube_Only(LPDIRECT3DDEVICE9 pGraphic_Device) : CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cube_Only::CVIBuffer_Cube_Only(const CVIBuffer_Cube_Only& Prototype) : CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Cube_Only::Initialize_Prototype(CUBE_ONLY& tInfo)
{
    m_iNumVertices = 24;
    m_iVertexStride = sizeof(VTXPOSTEX);
    m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1;
    m_iNumPritimive = 12;

    m_iNumIndices = m_iNumPritimive * 3;
    m_iIndexStride = 4;
    m_eIndexFormat = D3DFMT_INDEX32;
    
    _float3 fBoxSize = tInfo.BoxSIze;

    /*VB*/
    if (FAILED(__super::Create_VertexBuffer()))
        return E_FAIL;

    _float3 VertInex[8] =
    {
        // 앞. (0~3)
        {_float3(fBoxSize.x * -0.5f,   fBoxSize.y * 0.5f,  fBoxSize.z * -0.5f)},
        {_float3(fBoxSize.x * 0.5f,   fBoxSize.y * 0.5f,  fBoxSize.z * -0.5f)},
        {_float3(fBoxSize.x * 0.5f,   fBoxSize.y * -0.5f,  fBoxSize.z * -0.5f)},
        {_float3(fBoxSize.x * -0.5f,    fBoxSize.y * -0.5f,   fBoxSize.z * -0.5f)},

        // 뒤. (4~7)
        {_float3(fBoxSize.x * -0.5f,   fBoxSize.y * 0.5f,   fBoxSize.z * 0.5f)},
        {_float3(fBoxSize.x * 0.5f,   fBoxSize.y * 0.5f,  fBoxSize.z * 0.5f)},
        {_float3(fBoxSize.x * 0.5f,    fBoxSize.y * -0.5f,  fBoxSize.z * 0.5f)},
        {_float3(fBoxSize.x * -0.5f,    fBoxSize.y * -0.5f,   fBoxSize.z * 0.5f)},
    };

    VTXPOSTEX* pVertices = { nullptr };
    m_pVB->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);

#pragma region 버택스 점찍기
    // front 앞 면
    pVertices[0].vPosition = VertInex[0];
    pVertices[0].vTexcoord = {0.f, 0.f};
    pVertices[1].vPosition = VertInex[1];
    pVertices[1].vTexcoord = { 1.f, 0.f };
    pVertices[2].vPosition = VertInex[2];
    pVertices[2].vTexcoord = { 1.f, 1.f };
    pVertices[3].vPosition = VertInex[3];
    pVertices[3].vTexcoord = { 0.f, 1.f };

    // Right 오른쪽 면
    pVertices[4].vPosition = VertInex[1];
    pVertices[4].vTexcoord = { 0.f, 0.f };
    pVertices[5].vPosition = VertInex[5];
    pVertices[5].vTexcoord = { 1.f, 0.f };
    pVertices[6].vPosition = VertInex[6];
    pVertices[6].vTexcoord = { 1.f, 1.f };
    pVertices[7].vPosition = VertInex[2];
    pVertices[7].vTexcoord = { 0.f, 1.f };

    // back 뒷면
    pVertices[8].vPosition = VertInex[4];
    pVertices[8].vTexcoord = { 0.f, 0.f };
    pVertices[9].vPosition = VertInex[5];
    pVertices[9].vTexcoord = { 1.f, 0.f };
    pVertices[10].vPosition = VertInex[6];
    pVertices[10].vTexcoord = { 1.f, 1.f };
    pVertices[11].vPosition = VertInex[7];
    pVertices[11].vTexcoord = { 0.f, 1.f };

    // left 왼쪽면
    pVertices[12].vPosition = VertInex[4];
    pVertices[12].vTexcoord = { 0.f, 0.f };
    pVertices[13].vPosition = VertInex[0];
    pVertices[13].vTexcoord = { 1.f, 0.f };
    pVertices[14].vPosition = VertInex[3];
    pVertices[14].vTexcoord = { 1.f, 1.f };
    pVertices[15].vPosition = VertInex[7];
    pVertices[15].vTexcoord = { 0.f, 1.f };

    // Up 윗면
    pVertices[16].vPosition = VertInex[4];
    pVertices[16].vTexcoord = { 0.f, 0.f };
    pVertices[17].vPosition = VertInex[5];
    pVertices[17].vTexcoord = { 1.f, 0.f };
    pVertices[18].vPosition = VertInex[1];
    pVertices[18].vTexcoord = { 1.f, 1.f };
    pVertices[19].vPosition = VertInex[0];
    pVertices[19].vTexcoord = { 0.f, 1.f };

    // Down 아랫면
    pVertices[20].vPosition = VertInex[3];
    pVertices[20].vTexcoord = { 0.f, 0.f };
    pVertices[21].vPosition = VertInex[2];
    pVertices[21].vTexcoord = { 1.f, 0.f };
    pVertices[22].vPosition = VertInex[6];
    pVertices[22].vTexcoord = { 1.f, 1.f };
    pVertices[23].vPosition = VertInex[7];
    pVertices[23].vTexcoord = { 0.f, 1.f };

#pragma endregion

    m_pVB->Unlock();

    /*IB*/
    if (FAILED(__super::Create_IndexBuffer()))
        return E_FAIL;

    _uint		iNumIndices = { 0 };
    _uint* pIndices = { nullptr };

    m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

    // 앞.
    pIndices[iNumIndices++] = 0;
    pIndices[iNumIndices++] = 1;
    pIndices[iNumIndices++] = 2;
    pIndices[iNumIndices++] = 0;
    pIndices[iNumIndices++] = 2;
    pIndices[iNumIndices++] = 3;

    // 뒤.
    pIndices[iNumIndices++] = 9;  
    pIndices[iNumIndices++] = 8;  
    pIndices[iNumIndices++] = 11;
    pIndices[iNumIndices++] = 9;  
    pIndices[iNumIndices++] = 11;  
    pIndices[iNumIndices++] = 10;

    // 우측면.
    pIndices[iNumIndices++] = 4;  
    pIndices[iNumIndices++] = 5;  
    pIndices[iNumIndices++] = 6;
    pIndices[iNumIndices++] = 4;  
    pIndices[iNumIndices++] = 6; 
    pIndices[iNumIndices++] = 7;

    // 왼측면.
    pIndices[iNumIndices++] = 12;  
    pIndices[iNumIndices++] = 13;  
    pIndices[iNumIndices++] = 14;
    pIndices[iNumIndices++] = 12; 
    pIndices[iNumIndices++] = 14;
    pIndices[iNumIndices++] = 15;

    // 위
    pIndices[iNumIndices++] = 16;
    pIndices[iNumIndices++] = 17; 
    pIndices[iNumIndices++] = 18;
    pIndices[iNumIndices++] = 16;  
    pIndices[iNumIndices++] = 18;  
    pIndices[iNumIndices++] = 19;

    // 아래
    pIndices[iNumIndices++] = 20;  
    pIndices[iNumIndices++] = 21;  
    pIndices[iNumIndices++] = 22;
    pIndices[iNumIndices++] = 20;  
    pIndices[iNumIndices++] = 22;  
    pIndices[iNumIndices++] = 23;

    m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Cube_Only::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Cube_Only* CVIBuffer_Cube_Only::Create(LPDIRECT3DDEVICE9 pGraphic_Device, CUBE_ONLY tInfo)
{
    CVIBuffer_Cube_Only* pInstance = new CVIBuffer_Cube_Only(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(tInfo)))
    {
        MSG_BOX("Failed to Created : CVIBuffer_Cube_Only");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Cube_Only::Clone(void* pArg)
{
    CVIBuffer_Cube_Only* pInstance = new CVIBuffer_Cube_Only(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Clone : CVIBuffer_Cube_Only");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVIBuffer_Cube_Only::Free()
{
	__super::Free();
}
