#include "Loader.h"

#include "GameInstance.h"

#include "Camera_Free.h"
#include "Camera_Player.h"

#include "BackGround.h"
#include "Terrain.h"
#include "player.h"

#include "Tool.h"

//Moo
#include "Creeper.h"
#include "Steve.h"
#include "Tree.h"
#include "Wood.h"
#include "Leaf.h"
#include "Sword.h"

//HERO
#include "HeroCube.h" //콜라이더 테스트용 큐브
#include "HeroCubeNoMove.h"
#include "HeroEnemy.h"
#include "SkyBox.h"


//지형 관련
#include "Dirt.h"
#include "Stone.h"
#include "GrassDirt.h"
#include "CoalOre.h"
#include "IronOre.h"
#include "MCTerrain.h"
#include "MapTool.h"

//Hyock
#include "HyockCube.h"
#include "VIBuffer_Cube_Only.h"
#include "ParticleRain.h"
#include "ParticleDash.h"

//Woo
#include "MainLogo.h"
#include "StartButton.h"
#include "Title.h"
#include "Edition.h"


CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	/* 자원로딩한다. */
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;	

	return 0;

}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}


HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo();
		break;

	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;

	case LEVEL_MOO:
		hr = Loading_For_MOOPlay();
		break;

	case LEVEL_HERO:
		hr = Loading_For_HEROPlay();
		break;

	case LEVEL_WOO:
		hr = Loading_For_WOOPlay();
		break;

	case LEVEL_YU:
		hr = Loading_For_YUPlay();
		break;

	case LEVEL_HYEOK:
		hr = Loading_For_HECKPlay();
		break;

	case LEVEL_TOOL:
		hr = Loading_For_TOOL();
		break;

	}

	LeaveCriticalSection(&m_CriticalSection); //오류 나서 두개 위치 바꿈

	if (FAILED(hr))
		return E_FAIL;	//오류 나서 두개 위치 바꿈

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_BackGround*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩딩중입니다."));
	

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩딩중입니다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

#pragma region MODEL
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));

	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 256, 256))))
		return E_FAIL;

	/*-----------------------------------
	*  크리퍼 모델
	-----------------------------*/
	/* For.Prototype_Component_Texture_Creeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Creeper"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Creeper/Creeper.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Creeper */
	Engine::CUBE cube{ _float2(64.f, 32.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Creeper_Head"),
		CVIBuffer_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 32.f), _float3(6.f, 12.f, 4.f), _float2(16.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Creeper_Body"),
		CVIBuffer_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 32.f), _float3(4.f, 6.f, 4.f), _float2(0.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
		CVIBuffer_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Creeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Creeper"),
		CCreeper::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_MOOPlay()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

	// 나무 텍스쳐
	/* For.Prototype_Component_Texture_Leaf */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Texture_Leaf"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/leaves0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Texture_Wood"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/wood0.png"), 1))))
		return E_FAIL;

	// 철검 택스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Texture_Sword"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Sword/iron_sword.png"), 1))))
		return E_FAIL;


	// 돌 곡괭이 택스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Texture_Pickaxe"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Pickaxe/stone_pickaxe.png"), 1))))
		return E_FAIL;
	

	/*-----------------------------------
	*  크리퍼 모델
	-----------------------------*/
	/* For.Prototype_Component_Texture_Creeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Texture_Creeper"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Creeper/Creeper.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Creeper */
	Engine::CUBE cube{ _float2(64.f, 32.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_VIBuffer_Creeper_Head"),
		CVIBuffer_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 32.f), _float3(6.f, 12.f, 4.f), _float2(16.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_VIBuffer_Creeper_Body"),
		CVIBuffer_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 32.f), _float3(4.f, 6.f, 4.f), _float2(0.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
		CVIBuffer_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 터레인 큐브
	//cube = { _float2(64.f, 32.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f) };
	///* For.Prototype_Component_Com_mVIBCubeTerrain */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Com_mVIBCubeTerrain"),
	//	CVIBuffer_CubeTerrain::Create(m_pGraphic_Device, cube))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Creeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper"),
		CCreeper::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Steve */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Steve"),
		CSteve::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Wood"),
		CWood::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Leaf */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Leaf"),
		CLeaf::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tree */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Sword */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Sword"),
		CSword::Create(m_pGraphic_Device))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_YUPlay()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_Dirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Dirt"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/dirt%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GrassDirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_GrassDirt"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/grassDirt%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Stone"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/stone%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CoalOre */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_CoalOre"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/coalOre%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_IronOre */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_IronOre"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/ironOre%d.png"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	Engine::CUBE cube{ _float2(64.f, 32.f), _float3(16.f, 16.f, 16.f), _float2(0.f, 0.f) };
	/* For.Prototype_Component_VIBuffer_CubeInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_CubeInstance"),
		CVIBuffer_CubeInstance::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Dirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Dirt"),
		CDirt::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_GrassDirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_GrassDirt"),
		CGrassDirt::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Stone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Stone"),
		CStone::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_CoalOre */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_CoalOre"),
		CCoalOre::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_IronOre */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_IronOre"),
		CIronOre::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_MCTerrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_MCTerrain"),
		CMCTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tool */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_MapTool"),
		CMapTool::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_HEROPlay()
{
#pragma region TEXTURE
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	/* For.Prototype_Component_Texture_BackGround*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GrassDirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_Texture_GrassDirt"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/grassDirt%d.png"), 1))))
		return E_FAIL;
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("콜라이더을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("리지드바디을(를) 로딩중입니다."));
	/* For.Prototype_Component_Rigidbody */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_Rigidbody"),
		CRigidbody::Create(m_pGraphic_Device))))
		return E_FAIL;
 	
#pragma region MODEL
	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	/* For.Prototype_GameObject_Camera_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_HeroCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_HeroCube"),
		CHeroCube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_HeroCubeNoMove */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_HeroCubeNoMove"),
		CHeroCubeNoMove::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_HeroEnemy */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_HeroEnemy"),
		CHeroEnemy::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_SkyBox */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_SkyBox"),
		CSkyBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_WOOPlay()
{
#pragma region Add_Prototype Component
	
	#pragma region Texture UI
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	///* For.Prototype_Component_Texture_Loading*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_Component_Texture_Loading"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/Loading/loading_%03d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_MainLogo*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_Component_Texture_MainLogo"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/MainLogo/MinecraftBanner.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_StartButton*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_Component_Texture_StartButton"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/StartButton/StartButton1.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_title*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_Component_Texture_title"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/title/minecraft.png"), 1))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_edition*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_Component_Texture_edition"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/title/edition.png"), 1))))
		return E_FAIL;

	#pragma endregion 

#pragma endregion 
	
#pragma region Add_Prototype Game Object

	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	#pragma region UI Object
	/* For.Prototype_GameObject_MainLogo */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_GameObject_MainLogo"),
		CMainLogo::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_StartButton */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_GameObject_StartButton"),
		CStartButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_title */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_GameObject_title"),
		CTitle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Edition */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_WOO, TEXT("Prototype_GameObject_Edition"),
		CEdition::Create(m_pGraphic_Device))))
		return E_FAIL;


	#pragma endregion 

#pragma endregion 



	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_HECKPlay()
{
	lstrcpy(m_szLoadingText, TEXT(" 텍스쳐을(를) 로딩중입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Destroy"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Effect/destroy_stage_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Smoke"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/big_smoke_4.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Rain"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/rain.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Dash"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Particle/dash.png"), 1))))
		return E_FAIL;
  
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));

	Engine::CUBE_ONLY cube{ _float3(1.f, 1.f, 1.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_Component_Cube_Destroy"),
		CVIBuffer_Cube_Only::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_Component_Cube_Diamond_Ore"),
		CVIBuffer_Cube_Only::Create(m_pGraphic_Device, cube))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_GameObject_Hyock_Cube"),
		CHyockCube::Create(m_pGraphic_Device))))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_GameObject_ParticleRain"),
		CParticleRain::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, TEXT("Prototype_GameObject_ParticleDash"),
		CParticleDash::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT(" 원형객체을(를) 로딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_TOOL()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tool */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Tool"),
		CTool::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

CLoader* CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pGraphic_Device);


	DeleteCriticalSection(&m_CriticalSection);
}
