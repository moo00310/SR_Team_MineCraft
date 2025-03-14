#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "MapTool.h"
#include "GameInstance.h"
#include "MCTerrain.h"

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

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

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
        ImGui::SetNextWindowSize(ImVec2(300, 300));
        ImGui::Begin("Map Tool");

        if (ImGui::Button("Basic Terrain", ImVec2(150, 150))) {
            m_bMainFrame = false;
            m_bBasicFrame = true;
        }

        if (ImGui::Button("Make Height Map", ImVec2(150, 150))) {
            m_bMainFrame = false;
            m_bMapFrame = true;
        }

        ImGui::End();
    }
#pragma endregion

#pragma region BasicFrame
    if (m_bBasicFrame) {
        ImGui::SetNextWindowSize(ImVec2(400, 400));
        ImGui::Begin("BasicMap Tool");

        if (ImGui::Button("To Main", ImVec2(200, 50))) {
            m_bBasicFrame = false;
            m_bMainFrame = true;
        }

        ImGui::InputInt("Map X", &m_iMapX);
        ImGui::InputInt("Map Y", &m_iMapY);
        ImGui::InputInt("Map Z", &m_iMapZ);

        if (ImGui::Button("Generation", ImVec2(200, 50))) {
            TerrainGeneration();
        }

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


        if (ImGui::Button("Show Height Gray Img", ImVec2(200, 50))) {
            if (heightMapTexture) {
                heightMapTexture->Release();
                heightMapTexture = nullptr;
            }
            GeneratePerlinNoiseTexture(128, 128);
            m_bMapHeightFrame = true;
        }

        if (ImGui::Button("Show Terrain Img", ImVec2(200, 50))) {
            if (ColorMapTexture) {
                ColorMapTexture->Release();
                ColorMapTexture = nullptr;
            }
            GeneratePerlinNoiseTextureColor(128, 128);
            m_bMapColorFrame = true;
        }

        if (ImGui::Button("Generation", ImVec2(200, 50))) {
            TerrainGenerationWithNoise();
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

    if (m_bMapColorFrame) {
        ImGui::Begin("Color Map 2D", &m_bMapColorFrame);
        if (ColorMapTexture) {
            ImGui::Image((ImTextureID)ColorMapTexture, ImVec2(256, 256));
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

HRESULT CMapTool::TerrainGeneration()
{
    m_pGameInstance->ClearLayer(LEVEL_YU, TEXT("Layer_Environment"));

    //2차원 
/*
    for (int i = 0; i < m_iMapX; ++i) {
    for (int j = 0; j < m_iMapZ; ++j) {
        if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Dirt"),
            LEVEL_YU, strLayerTag)))
            return E_FAIL;

        _float3 temp = { (float)j,0.f, (float)i };
        dynamic_cast<CDirt*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_BackGround"), i * m_iMapZ + j))->SetPos(temp);
    }
}
*/


// 3차원
    for (int i = 0; i < m_iMapY; ++i) {  // Y축이 높이
        for (int j = 0; j < m_iMapX; ++j) {  // X축
            for (int k = 0; k < m_iMapZ; ++k) {  // Z축
                if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_GrassDirt"),LEVEL_YU, TEXT("Layer_Environment"))))
                    return E_FAIL;


                _float3 temp = { (float)j,(float)i,(float)k };
                dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Environment"), (i * m_iMapX * m_iMapZ) + (j * m_iMapZ) + k))->SetPos(temp);
            }
        }
    }

    return S_OK;
}

HRESULT CMapTool::TerrainGenerationWithNoise()
{
    m_pGameInstance->ClearLayer(LEVEL_YU, TEXT("Layer_Environment"));

    D3DLOCKED_RECT heightLockedRect;
    D3DLOCKED_RECT colorLockedRect;

    int index = 0;

    if (SUCCEEDED(heightMapTexture->LockRect(0, &heightLockedRect, NULL, D3DLOCK_READONLY)) &&
        SUCCEEDED(ColorMapTexture->LockRect(0, &colorLockedRect, NULL, D3DLOCK_READONLY))) {

        DWORD* heightPixels = (DWORD*)heightLockedRect.pBits;
        DWORD* colorPixels = (DWORD*)colorLockedRect.pBits;

        int pitchHeight = heightLockedRect.Pitch / 4; // DWORD(4바이트) 단위 변환
        int pitchColor = colorLockedRect.Pitch / 4;

        int width = 128;  // 텍스처 가로 크기
        int height = 128; // 텍스처 세로 크기

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // 높이맵 픽셀 값 가져오기
                DWORD heightColor = heightPixels[y * pitchHeight + x];
                int heightValue = (heightColor & 0xFF)/15;

                // 컬러맵 픽셀 값 가져오기
                DWORD colorValue = colorPixels[y * pitchColor + x];
                int b = (colorValue) & 0xFF;

                if (b == 34) {
                    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_GrassDirt"), LEVEL_YU, TEXT("Layer_Environment"))))
                        return E_FAIL;
                }
                else {
                    if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Stone"), LEVEL_YU, TEXT("Layer_Environment"))))
                        return E_FAIL;
                }

                

                _float3 temp = { (float)x,(float)heightValue,(float)y };
                dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Environment"), index))->SetPos(temp);
                index++;
            }
        }

        // 두 개의 텍스처 해제
        heightMapTexture->UnlockRect(0);
        ColorMapTexture->UnlockRect(0);
    }
    return S_OK;
}

void CMapTool::GeneratePerlinNoiseTexture(int width, int height) {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
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

void CMapTool::GeneratePerlinNoiseTextureColor(int width, int height)
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(m_fFrequency);
    noise.SetSeed(m_iSeed);

    // DirectX9 텍스처 생성
    if (FAILED(m_pGraphic_Device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &ColorMapTexture, NULL))) {
        return;
    }

    D3DLOCKED_RECT lockedRect;
    if (FAILED(ColorMapTexture->LockRect(0, &lockedRect, NULL, 0))) {
        ColorMapTexture->Release();
        return;
    }

    // 픽셀 데이터 채우기
    DWORD* pixels = (DWORD*)lockedRect.pBits;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float noiseValue = noise.GetNoise((float)x, (float)y);
            int gray = (int)((noiseValue + 1.0f) * 127.5f); // [-1, 1] → [0, 255]
            int threshold = 110; // 땅과 물을 구분하는 기준값 (조절 가능)

            if (gray > threshold) {
                pixels[y * (lockedRect.Pitch / 4) + x] = D3DCOLOR_ARGB(255, 34, 139, 34);  // 초록색 
            }
            else {
                pixels[y * (lockedRect.Pitch / 4) + x] = D3DCOLOR_ARGB(255, 219, 204, 163); // 모래색 
            }
        }
    }

    ColorMapTexture->UnlockRect(0);
    return;
}

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
   if (ColorMapTexture) {
       ColorMapTexture->Release();
       ColorMapTexture = nullptr;
    }
}
