﻿#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "MapTool.h"
#include "GameInstance.h"
#include "MCTerrain.h"

#include "Client_Struct.h"

CMapTool::CMapTool(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTool{ pGraphic_Device }
{

}

CMapTool::CMapTool(const CMapTool& Prototype)
	: CTool(Prototype)
{

}

HRESULT CMapTool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapTool::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	ImGui::StyleColorsDark();

	return S_OK;
}

void CMapTool::Priority_Update(_float fTimeDelta)
{
}

void CMapTool::Update(_float fTimeDelta)
{

}

void CMapTool::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CMapTool::Render()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

#pragma region MainFrame
    if (m_bMainFrame) {
        ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::Begin("Map Tool");

        if (ImGui::Button("X -"))
            m_iMapX = max(4, m_iMapX - 16);
        ImGui::SameLine();
        ImGui::Text("MapX Size: %d", m_iMapX);
        ImGui::SameLine();
        if (ImGui::Button("X +"))
            m_iMapX = min(256, m_iMapX + 16);

        if (ImGui::Button("Z -"))
            m_iMapZ = max(4, m_iMapZ - 16);
        ImGui::SameLine();
        ImGui::Text("MapZ Size: %d", m_iMapZ);
        ImGui::SameLine();
        if (ImGui::Button("Z +"))
            m_iMapZ = min(256, m_iMapZ + 16);

        if (ImGui::Button("Make Height Map", ImVec2(150, 50))) {
            m_bMainFrame = false;
            m_bMapFrame = true;
        }

        if (ImGui::Button("Make Cave Map", ImVec2(150, 50))) {
            m_bMainFrame = false;
            m_bCaveFrame = true;
        }

        if (ImGui::Button("Generation", ImVec2(200, 50))) {
            TerrainGenerationWithNoise();
        }
        ImGui::SameLine();
        ImGui::Text("less than 64x64 Good");

        if (ImGui::Button("SaveData", ImVec2(200, 50))) {
            SaveData();
        }
        ImGui::SameLine();
        ImGui::Text("Save To File");

        ImGui::End();
    }
#pragma endregion

#pragma region MapFrame
    if (m_bMapFrame) {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        ImGui::Begin("Height Map");

        if (ImGui::Button("To Main", ImVec2(200, 50))) {
            m_bMapFrame = false;
            m_bMainFrame = true;
        }

        ImGui::SliderInt("Seed", &m_iSeed, 0, 99999);
        ImGui::SliderFloat("Frequency", &m_fFrequency, 0.001f, 0.1f);

        ImGui::SliderInt("DirtDeep", &m_iDirtDeep, 0, 5);
        ImGui::SliderInt("MapDeep", &m_iMapDeep, -10, 15);

        if (ImGui::Button("Show Height Gray Img", ImVec2(200, 50))) {
            if (heightMapTexture) {
                heightMapTexture->Release();
                heightMapTexture = nullptr;
            }
            GeneratePerlinNoiseTexture(m_iMapX, m_iMapZ);
            m_bMapHeightFrame = true;
        }

        ImGui::End();
    }
#pragma endregion

#pragma region CaveFrame
    if (m_bCaveFrame) {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        ImGui::Begin("Cave Map");

        if (ImGui::Button("To Main", ImVec2(200, 50))) {
            m_bCaveFrame = false;
            m_bMainFrame = true;
        }

        ImGui::SliderInt("Seed", &m_icaveSeed, 0, 99999);
        ImGui::SliderFloat("Frequency", &m_fcaveFrequency, 0.001f, 0.1f);

        if (ImGui::Button("Show Cave Gray Img", ImVec2(200, 50))) {
            if (caveTexture) {
                caveTexture->Release();
                caveTexture = nullptr;
            }
            GeneratePerlinNoiseTextureCave(m_iMapX, m_iMapZ);
            m_bMapCaveFrame = true;
        }

        ImGui::End();
    }
#pragma endregion

#pragma region 2D이미지창 관련
    if (m_bMapHeightFrame) {
        ImGui::Begin("Height Map 2D", &m_bMapHeightFrame);
        if (heightMapTexture) {
            ImGui::Image((ImTextureID)heightMapTexture, ImVec2(256, 256));
        }
        ImGui::End();
    }

    if (m_bMapCaveFrame) {
        ImGui::Begin("Cave Map 2D", &m_bMapCaveFrame);
        if (m_bMapCaveFrame) {
            ImGui::Image((ImTextureID)caveTexture, ImVec2(256, 256));
        }
        ImGui::End();
    }
#pragma endregion

    ImGui::EndFrame();
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return S_OK;
}

BLOCKTYPE GetBlockType(int heightValue) {
    struct BlockProbability {
        int minHeight, maxHeight;
        int probability;
        BLOCKTYPE oreType;
    };

    static const BlockProbability blockTable[] = {
        { -3, -1, 3, COALORE },   // 석탄 5%
        { -6, -4, 7, IRONORE }    // 철광석 10%
    };

    for (const auto& entry : blockTable) {
        if (entry.minHeight <= heightValue && heightValue <= entry.maxHeight) {
            if (rand() % 100 < entry.probability) {
                return entry.oreType;
            }
        }
    }

    return STONE; // 기본값: 돌
}

// 지형 생성 함수 
HRESULT CMapTool::TerrainGenerationWithNoise()
{
    m_pGameInstance->ClearLayer(LEVEL_TOOL, TEXT("Layer_Environment"));

    D3DLOCKED_RECT heightLockedRect;
    D3DLOCKED_RECT caveLockedRect;

    int index = 0;

    vector<_float3> m_vecGrassDirt;
    vector<_float3> m_vecDirt;
    vector<_float3> m_vecStone;
    vector<_float3> m_vecCoal;
    vector<_float3> m_vecIron;

    if (SUCCEEDED(heightMapTexture->LockRect(0, &heightLockedRect, NULL, D3DLOCK_READONLY))) {
        if (SUCCEEDED(caveTexture->LockRect(0, &caveLockedRect, NULL, D3DLOCK_READONLY))) {

            DWORD* heightPixels = (DWORD*)heightLockedRect.pBits;
            DWORD* cavePixels = (DWORD*)caveLockedRect.pBits;

            int pitchHeight = heightLockedRect.Pitch / 4;
            int pitchCave = caveLockedRect.Pitch / 4;

            const int width = m_iMapX;
            const int height = m_iMapZ;
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    DWORD heightColor = heightPixels[y * pitchHeight + x];
                    int heightValue = (heightColor & 0xFF) / 15;

                    DWORD caveColor = cavePixels[y * pitchCave + x];
                    int caveValue = (caveColor & 0xFF) /15 + m_iDirtDeep + 10;

                    if (caveValue != heightValue) {
                        m_vecGrassDirt.push_back(_float3((float)x, (float)heightValue, (float)y));
                    }

                    int depth = m_iDirtDeep;
                    int minDepth = m_iMapDeep;
                    while (heightValue > minDepth) {
                        heightValue--;
                        BLOCKDESC eblockData2;
                        eblockData2.fPosition = _float3((float)x, (float)heightValue, (float)y);
                        eblockData2.eBlockType = (depth > 0) ? DIRT : STONE;

                        switch (eblockData2.eBlockType)
                        {
                        case DIRT:
                            if (caveValue != heightValue) {
                                m_vecDirt.push_back(_float3((float)x, (float)heightValue, (float)y));
                            }
                            break;
                        case STONE:
                            if (caveValue != heightValue) {
                                m_vecStone.push_back(_float3((float)x, (float)heightValue, (float)y));
                            }
                            break;
                        case IRONORE:
                            m_vecIron.push_back(_float3((float)x, (float)heightValue, (float)y));
                            break;
                        case COALORE:
                            m_vecCoal.push_back(_float3((float)x, (float)heightValue, (float)y));
                            break;
                        default:
                            break;
                        }
                        depth--;
                    }
                }
            }
            caveTexture->UnlockRect(0);
        }
        // 텍스처 해제
        heightMapTexture->UnlockRect(0);
    }

     vector<pair<const wchar_t*, vector<D3DXVECTOR3>*>> blockTypes = {
        {TEXT("Prototype_GameObject_GrassDirt"), &m_vecGrassDirt},
        {TEXT("Prototype_GameObject_Dirt"), &m_vecDirt},
        {TEXT("Prototype_GameObject_Stone"), &m_vecStone},
        {TEXT("Prototype_GameObject_IronOre"), &m_vecIron},
        {TEXT("Prototype_GameObject_CoalOre"), &m_vecCoal}
     };


     for (size_t i = 0; i < blockTypes.size(); ++i)
     {
         m_pGameInstance->Add_GameObject(LEVEL_TOOL, blockTypes[i].first, LEVEL_TOOL, TEXT("Layer_Environment"));
         CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_TOOL, TEXT("Layer_Environment"), static_cast<int>(i)));
         if (pCube) {
             pCube->Set_InstanceBuffer(*(blockTypes[i].second), 1.f);
             //pCube->Set_MyChunk(i);
             //pCube->Set_BlockPositions(*(blockTypes[i].second));
         }
     }

    return S_OK;
}

// 높이 텍스처 생성 함수
void CMapTool::GeneratePerlinNoiseTexture(int width, int height) {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    //noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(m_fFrequency);
    noise.SetSeed(m_iSeed);

    // DirectX9 텍스처 생성
    if (FAILED(m_pGraphic_Device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &heightMapTexture, NULL))) {
        return;
    }

    D3DLOCKED_RECT lockedRect;
    if (FAILED(heightMapTexture->LockRect(0, &lockedRect, NULL, 0))) {
        heightMapTexture->Release();
        return;
    }

    // 픽셀 데이터 채우기
    DWORD* pixels = (DWORD*)lockedRect.pBits;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float noiseValue = noise.GetNoise((float)x, (float)y);
            int gray = (int)((noiseValue + 1.0f) * 127.5f); // [-1, 1] → [0, 255]

            pixels[y * (lockedRect.Pitch / 4) + x] = D3DCOLOR_ARGB(255, gray, gray, gray);
        }
    }

    heightMapTexture->UnlockRect(0);
    return;
}

// 동굴 텍스처 생성 함수
void CMapTool::GeneratePerlinNoiseTextureCave(int width, int height) {
    FastNoiseLite noise;
    //noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(m_fcaveFrequency);
    noise.SetSeed(m_icaveSeed);

    // DirectX9 텍스처 생성
    if (FAILED(m_pGraphic_Device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &caveTexture, NULL))) {
        return;
    }

    D3DLOCKED_RECT lockedRect;
    if (FAILED(caveTexture->LockRect(0, &lockedRect, NULL, 0))) {
        caveTexture->Release();
        return;
    }

    // 픽셀 데이터 채우기
    DWORD* pixels = (DWORD*)lockedRect.pBits;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float noiseValue = noise.GetNoise((float)x, (float)y);
            int gray = (int)((noiseValue + 1.0f) * 127.5f); // [-1, 1] → [0, 255]

           int chk =  gray / 15;

            if (chk==4 || chk == 5 || chk == 6) {
                pixels[y * (lockedRect.Pitch / 4) + x] = D3DCOLOR_ARGB(255, 0, 255, gray);
            }
            else {
                pixels[y * (lockedRect.Pitch / 4) + x] = D3DCOLOR_ARGB(255, gray, gray, gray);
            }
            
        }
    }

    caveTexture->UnlockRect(0);
    return;
}

#pragma region 파일 저장 with 쓰레드
CRITICAL_SECTION cs2;

struct SaveThreadParams {
    int chunkX, chunkZ;
    int numChunksX;
    DWORD* heightPixels;
    int pitchHeight;
    DWORD* cavePixels;
    int pitchCave;
    int m_iDirtDeep, m_iMapDeep, m_iStoneDeep, m_iCoalDeep;
};

DWORD WINAPI SaveChunkThread(LPVOID lpParam) {
    SaveThreadParams* params = (SaveThreadParams*)lpParam;
    int chunkIndex = params->chunkZ * params->numChunksX + params->chunkX;

    wchar_t filename[100];
    swprintf(filename, 100, L"../bin/Resources/DataFiles/BlockDataChunk%d.txt", chunkIndex);

    HANDLE hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return 1;
    }
    DWORD dwBytesWritten;

    vector<BLOCKDESC> m_vGrassDirt;
    vector<BLOCKDESC> m_vDirt;
    vector<BLOCKDESC> m_vStone;
    vector<BLOCKDESC> m_vIron;
    vector<BLOCKDESC> m_vCoal;

    for (int y = params->chunkZ * 16; y < (params->chunkZ + 1) * 16; y++) {
        for (int x = params->chunkX * 16; x < (params->chunkX + 1) * 16; x++) {
            DWORD heightColor = params->heightPixels[y * params->pitchHeight + x];
            int heightValue = (heightColor & 0xFF) / 15;

            BLOCKDESC eblockData;
            eblockData.eBlockType = GRASSDIRT;
            eblockData.fPosition = _float3((float)x, (float)heightValue, (float)y);
            m_vGrassDirt.push_back(eblockData);

            int depth = params->m_iDirtDeep;
            int minDepth = params->m_iMapDeep;
            int stoneDepth = params->m_iStoneDeep;
            int coalDepth = params->m_iCoalDeep;

            while (heightValue > minDepth) {
                heightValue--;
                BLOCKDESC eblockData2;
                eblockData2.fPosition = _float3((float)x, (float)heightValue, (float)y);

                if (depth > 0) {
                    eblockData2.eBlockType = DIRT;
                    depth--;
                }
                else if (stoneDepth > 0) {
                    eblockData2.eBlockType = STONE;
                    stoneDepth--;
                }
                else if (coalDepth > 0) {
                    int random = rand() % 100;
                    if (random < 10) {
                        eblockData2.eBlockType = COALORE;
                        coalDepth--;
                    }
                    else {
                        eblockData2.eBlockType = STONE;
                        coalDepth--;
                    }
                }
                else {
                    int random = rand() % 100;
                    if (random < 7) {
                        eblockData2.eBlockType = IRONORE;
                    }
                    else {
                        eblockData2.eBlockType = STONE;
                    }
                }

                switch (eblockData2.eBlockType)
                {
                case DIRT:
                        m_vDirt.push_back(eblockData2);
                    break;
                case STONE:
                        m_vStone.push_back(eblockData2);
                    break;
                case IRONORE:
                        m_vIron.push_back(eblockData2);
                    break;
                case COALORE:
                    m_vCoal.push_back(eblockData2);
                    break;
                default:
                    break;
                }
            }
        }
    }

    for (auto desc : m_vGrassDirt) {
        WriteFile(hFile, &desc, sizeof(BLOCKDESC), &dwBytesWritten, NULL);
    }
    for (auto desc : m_vDirt) {
        WriteFile(hFile, &desc, sizeof(BLOCKDESC), &dwBytesWritten, NULL);
    }

    for (auto desc : m_vStone) {
        WriteFile(hFile, &desc, sizeof(BLOCKDESC), &dwBytesWritten, NULL);
    }

    for (auto desc : m_vIron) {
        WriteFile(hFile, &desc, sizeof(BLOCKDESC), &dwBytesWritten, NULL);
    }
    for (auto desc : m_vCoal) {
        WriteFile(hFile, &desc, sizeof(BLOCKDESC), &dwBytesWritten, NULL);
    }

    CloseHandle(hFile);
    return 0;
}

HRESULT CMapTool::SaveData() {
    InitializeCriticalSection(&cs2);

    const int CHUNK_SIZE = 16;
    const int numChunksX = m_iMapX / CHUNK_SIZE;
    const int numChunksZ = m_iMapZ / CHUNK_SIZE;

    D3DLOCKED_RECT heightLockedRect;
    D3DLOCKED_RECT caveLockedRect;
    if (FAILED(heightMapTexture->LockRect(0, &heightLockedRect, NULL, D3DLOCK_READONLY))) {
        return E_FAIL;
    }

    if (FAILED(caveTexture->LockRect(0, &caveLockedRect, NULL, D3DLOCK_READONLY))) {
        return E_FAIL;
    }


    DWORD* heightPixels = (DWORD*)heightLockedRect.pBits;
    int pitchHeight = heightLockedRect.Pitch / 4;

    DWORD* cavePixels = (DWORD*)caveLockedRect.pBits;
    int pitchCave = caveLockedRect.Pitch / 4;

    HANDLE* hThreads = new HANDLE[numChunksX * numChunksZ];
    SaveThreadParams* params = new SaveThreadParams[numChunksX * numChunksZ];

    for (int chunkZ = 0; chunkZ < numChunksZ; chunkZ++) {
        for (int chunkX = 0; chunkX < numChunksX; chunkX++) {
            int index = chunkZ * numChunksX + chunkX;
            params[index] = { chunkX, chunkZ, numChunksX, heightPixels, pitchHeight, cavePixels,pitchCave,m_iDirtDeep, m_iMapDeep,m_iStoneDeep ,m_iCoalDeep };

            hThreads[index] = CreateThread(NULL, 0, SaveChunkThread, &params[index], 0, NULL);
            if (!hThreads[index]) {
                delete[] hThreads;
                delete[] params;
                heightMapTexture->UnlockRect(0);
                return E_FAIL;
            }
        }
    }

    WaitForMultipleObjects(numChunksX * numChunksZ, hThreads, TRUE, INFINITE);

    for (int i = 0; i < numChunksX * numChunksZ; ++i) {
        CloseHandle(hThreads[i]);
    }

    delete[] hThreads;
    delete[] params;
    heightMapTexture->UnlockRect(0);
    DeleteCriticalSection(&cs2);

    MSG_BOX("모든 블록 데이터 처리가 완료되었습니다!");

    return S_OK;
}
#pragma endregion


CMapTool* CMapTool::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMapTool* pInstance = new CMapTool(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMapTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMapTool::Clone(void* pArg)
{
	CMapTool* pInstance = new CMapTool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CMapTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapTool::Free()
{
	__super::Free();
    if (heightMapTexture) {
        heightMapTexture->Release();
        heightMapTexture = nullptr;
    }
    if (caveTexture) {
        caveTexture->Release();
        caveTexture = nullptr;
    }
}
