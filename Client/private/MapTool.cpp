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

        if (ImGui::Button("Make Height Map", ImVec2(150, 150))) {
            m_bMainFrame = false;
            m_bMapFrame = true;
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

        if (ImGui::Button("X -"))
            m_iMapX = max(4, m_iMapX / 2);
        ImGui::SameLine();
        ImGui::Text("MapX Size: %d", m_iMapX);
        ImGui::SameLine();
        if (ImGui::Button("X +"))
            m_iMapX = min(256, m_iMapX * 2);

        if (ImGui::Button("Z -"))
            m_iMapZ = max(4, m_iMapZ / 2);
        ImGui::SameLine();
        ImGui::Text("MapZ Size: %d", m_iMapZ);
        ImGui::SameLine();
        if (ImGui::Button("Z +"))
            m_iMapZ = min(256, m_iMapZ * 2);

        ImGui::SliderInt("Seed", &m_iSeed, 0, 99999);
        ImGui::SliderFloat("Frequency", &m_fFrequency, 0.001f, 0.1f);



        if (ImGui::Button("Show Height Gray Img", ImVec2(200, 50))) {
            if (heightMapTexture) {
                heightMapTexture->Release();
                heightMapTexture = nullptr;
            }
            GeneratePerlinNoiseTexture(m_iMapX, m_iMapZ);
            m_bMapHeightFrame = true;
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
#pragma endregion

    ImGui::EndFrame();
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return S_OK;
}

// 지형 생성 함수 
HRESULT CMapTool::TerrainGenerationWithNoise()
{
    m_pGameInstance->ClearLayer(LEVEL_YU, TEXT("Layer_Environment"));

    D3DLOCKED_RECT heightLockedRect;

    int index = 0;

    if (SUCCEEDED(heightMapTexture->LockRect(0, &heightLockedRect, NULL, D3DLOCK_READONLY))){
       
        DWORD* heightPixels = (DWORD*)heightLockedRect.pBits;

        int pitchHeight = heightLockedRect.Pitch / 4;

        const int width = m_iMapX;
        const int height = m_iMapZ;

        vector<CBreakableCube*> createdObjects;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                DWORD heightColor = heightPixels[y * pitchHeight + x];
                int heightValue = (heightColor & 0xFF) / 15;

                // 지형 블록 추가 (지표면)
                if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_GrassDirt"), LEVEL_YU, TEXT("Layer_Environment"))))
                    return E_FAIL;

                // 생성된 객체를 캐싱해서 사용
                CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Environment"), index));
                if (pCube) {
                    pCube->SetPos(_float3((float)x, (float)heightValue, (float)y));
                    createdObjects.push_back(pCube);
                }
                index++;

                int a = 5;
                int kk = -5;

                while (heightValue > kk) {
                    heightValue--;
                    if (a > 0) {
                        if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Dirt"), LEVEL_YU, TEXT("Layer_Environment"))))
                            return E_FAIL;

                        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Environment"), index));
                        if (pCube) {
                            pCube->SetPos(_float3((float)x, (float)heightValue, (float)y));
                            createdObjects.push_back(pCube);
                        }
                    }
                    else {
                        if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Stone"), LEVEL_YU, TEXT("Layer_Environment"))))
                            return E_FAIL;

                        CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Environment"), index));
                        if (pCube) {
                            pCube->SetPos(_float3((float)x, (float)heightValue, (float)y));
                            createdObjects.push_back(pCube);
                        }
                    }

                    a--;
                    index++;

                }  
            }
        }

        // 텍스처 해제
        heightMapTexture->UnlockRect(0);
    }
    return S_OK;
}

// 높이 텍스처 생성 함수
void CMapTool::GeneratePerlinNoiseTexture(int width, int height) {
    FastNoiseLite noise;
    //noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
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
}
