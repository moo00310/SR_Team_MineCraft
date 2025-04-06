#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <iostream>

#include "MainApp.h"
#include "GameInstance.h"

#include "Level_Logo.h"
#include "Level_Loading.h"

#include "Steve.h"
#include "Arm_Steve.h"

#include "ParticleEventManager.h"

#include "UI_Mgr.h"
#include "ParticleRain.h"
#include "ParticleDash.h"
#include "ParticleSandMining.h"
#include "ParticleWoodMining.h"
#include "ParticleSandDestroy.h"
#include "UIParticleRain.h"
#include "ParticleExplosion.h"
#include "Mouse.h"
#include "ParticleDie.h"
#include "ParticleSwordFlame.h"
#include "ParticleFireCracker.h"
#include "ParticleCharging.h"
#include "ParticleAppleEating.h"
#include "ParticleStoneMining.h"

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	srand(unsigned int(time(0)));

	ENGINE_DESC		Desc{};
	Desc.hWnd = g_hWnd;
	Desc.isWindowed = true;
	Desc.iWinSizeX = g_iWinSizeX;
	Desc.iWinSizeY = g_iWinSizeY;
	Desc.iNumLevels = LEVEL_END;
	Desc.iNumCollisionGroups = COLLISION_END;

	if (FAILED(m_pGameInstance->Initialize_Engine(Desc, &m_pGraphic_Device)))
		return E_FAIL;

	CUI_Mgr::Get_Instance();

#pragma region 조명 연산
	D3DLIGHT9			LightDesc{};

	/* 난반사에 위한 빛의 색상. */
	LightDesc.Type = D3DLIGHT_DIRECTIONAL;
	LightDesc.Direction = _float3(1.f, -1.f, 1.f);
	LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//LightDesc.Specular = ;

	/* LightDesc.Specular = ;*/
	m_pGraphic_Device->SetLight(0, &LightDesc);

	/* 텍스쳐로 재질을 대체한다. */

	D3DMATERIAL9		MtrlDesc{};
	/* 난반사에 위한 픽셀의 색상. */
	MtrlDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlDesc.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
	//MtrlDesc.Specular = ;

	m_pGraphic_Device->SetMaterial(&MtrlDesc);

	m_pGraphic_Device->LightEnable(0, true);
#pragma endregion
	
	if (FAILED(Ready_Component_For_Static()))
		return E_FAIL;

	if (FAILED(Ready_Default_Setting()))
		return E_FAIL;

	/* 최초 보여줄 레벨을 할당하자. */
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;
	

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(m_pGraphic_Device);

	// 파티클 매니저 초기화.
	CParticleEventManager::Get_Instance()->Initialization(m_pGameInstance);

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);

	CUI_Mgr::Get_Instance()->Update(fTimeDelta);
	CUI_Mgr::Get_Instance()->Late_Update(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Draw();

	return S_OK;
}

HRESULT CMainApp::Ready_Default_Setting()
{
	// 샘플링
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//m_pGraphic_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	//m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

HRESULT CMainApp::Ready_Component_For_Static()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_Texture()))
		return E_FAIL;

	if (FAILED(Ready_Steve()))
		return E_FAIL;

	if (FAILED(Ready_Particle()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pGraphic_Device, eLevelID))))
		return E_FAIL;

	return S_OK;
}


HRESULT CMainApp::Ready_Steve()
{
	// 스티브 이미지
	/* For.Prototype_Component_Texture_Steve */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Steve"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Steve/Steve.png"), 1))))
		return E_FAIL;

	// 모델
	/* For.Prototype_Component_VIBuffer_Steve */
	Engine::CUBE cube{ _float2(64.f, 64.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Head"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 64.f), _float3(8.f, 12.f, 4.f), _float2(16.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Body"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 64.f), _float3(4.f, 12.f, 4.f), _float2(0.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Foot_R"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 64.f), _float3(4.f, 12.f, 4.f), _float2(16.f, 48.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Foot_L"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 64.f), _float3(4.f, 12.f, 4.f), _float2(40.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Arm_R"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 64.f), _float3(4.f, 12.f, 4.f), _float2(32.f, 48.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Arm_L"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Particle()
{
	// TODO : 파티클.
	// 비 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_RAIN,
		CParticleRain::Create(m_pGraphic_Device))))
		return E_FAIL;

	// UI 비 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_UI_PARTICLE_RAIN,
		CUIParticleRain::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 달리기 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_DASH,
		CParticleDash::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 흙 캐는 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_MINING,
		CParticleSandMining::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 나무 캐는 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING,
		CParticleWoodMining::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 돌 캐는 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING,
		CParticleStoneMining::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 흙 파괴 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_DESTROY,
		CParticleSandDestroy::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 폭발 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_EXPLOSION,
		CParticleExplosion::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 사망 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_DIE,
		CParticleDie::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 불꽃 검 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME,
		CParticleSwordFlame::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 폭죽 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_FIRE_CRACKER,
		CParticleFireCracker::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 차징 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_GLITTER,
		CParticleCharging::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 사과 먹는 파티클.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_EATING,
		CParticleAppleEating::Create(m_pGraphic_Device))))
		return E_FAIL;

	return S_OK;
}


HRESULT CMainApp::Ready_Texture()
{
	// 피카츄 이미지
	/* For.Prototype_Component_Texture_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Player"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Player/Player0.png"), 1))))
		return E_FAIL;

	// 지형 이미지
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

	// TODO : 파티클 텍스쳐.	
	// 더미 구름 텍스쳐. (쓸모없음 나중에 지울 것).
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Smoke"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/big_smoke_4.png"), 1))))
		return E_FAIL;

	// 비 텍스쳐.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_RAIN,
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/rain.png"), 1))))
		return E_FAIL;	

	// 이동 시 바닥 튀는 텍스쳐.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_DASH,
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/dash.png"), 1))))
		return E_FAIL;

	// TODO : 블럭 채굴/파괴 텍스쳐.	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_MINING,
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/mining_%d.png"), 3))))
		return E_FAIL;

	// TODO : 폭발 텍스쳐.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_EXPLOSION,
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Effect/explosion_%d.png"), 16))))
		return E_FAIL;

	// 사망 텍스쳐.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_DIE,
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/die_%d.png"), 3))))
		return E_FAIL;	

	// 불꽃 텍스쳐.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXTURE_FLAME,
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/flame.png"), 1))))
		return E_FAIL;

	// 플래시 텍스쳐.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_COMPONENT_TEXUTRE_GLITTER,
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/glitter_%d.png"), 8))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Component()
{
	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 256, 256))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	Engine::CUBE cube{ _float2(64.f, 32.f), _float3(16.f, 16.f, 16.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	/* Prototype_Component_Collider_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		CCollider_Cube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* Prototype_Component_Collider_Capsule */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Capsule"),
		CCollider_Capsule::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Rigidbody */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Rigidbody"),
		CRigidbody::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_SkeletalAnimator */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SkeletalAnimator"),
		CSkeletalAnimator::Create(m_pGraphic_Device))))
		return E_FAIL;

	//  애니메이션 & 노말이 들어간 렉트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Normal"),
		CVIBuffer_Anim_Rect::Create(m_pGraphic_Device))))
		return E_FAIL;

	cube = { _float2(64.f, 32.f), _float3(16.f, 16.f, 16.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Anim_Cube"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_CubeInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_CubeInstance"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_CubeInstance.hlsl")))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Rect3DInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect3DInstance"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect3DInstance.hlsl")))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Cube"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Cube.hlsl")))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Rect*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Rect.hlsl")))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_UI.hlsl")))))
		return E_FAIL;

	/* For 어쩌구 파티클 쉐이더 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, PROTOTYPE_COMPONENT_SHADER_PARTICLE,
		CShader::Create(m_pGraphic_Device, TEXT("../Bin/ShaderFiles/Shader_Particle.hlsl")))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	__super::Free();

	CUI_Mgr::Get_Instance()->Free();
	CUI_Mgr::Get_Instance()->Destroy_Instance();


	// 파티클 매니저 메모리 해제.
	CParticleEventManager::Get_Instance()->Free();
	CParticleEventManager::Get_Instance()->Destroy_Instance();

	CMouse::Get_Instance()->Destroy_Instance();

	// Cleanup
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Safe_Release(m_pGraphic_Device);

	m_pGameInstance->Release_Engine();

	/* 내멤버를 정리한다.*/	
	Safe_Release(m_pGameInstance);

}
