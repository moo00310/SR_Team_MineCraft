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
#include "Arm_Steve.h"
#include "Tree.h"
#include "Wood.h"
#include "Leaf.h"
#include "Zombi.h"
#include "Rect_Model.h"
#include "Cube_Model.h"
#include "Right_hand.h"

//HERO
#include "HeroCube.h" //콜라이더 테스트용 큐브
#include "HeroCubeNoMove.h"
#include "HeroEnemy.h"
#include "SkyBox.h"
#include "Sun.h"
#include "Clouds.h"


//지형 관련
#include "Dirt.h"
#include "Stone.h"
#include "GrassDirt.h"
#include "CoalOre.h"
#include "IronOre.h"
#include "MCTerrain.h"
#include "MapTool.h"
#include "Grass.h"
#include "RedTulip.h"
#include "ItemCube.h"
#include "ItemRect.h"

//Hyock
#include "HyockCube.h"
#include "VIBuffer_Cube_Only.h"
#include "DestroyCube.h"

//Woo
#include "MainLogo.h"
#include "StartButton.h"
#include "Title.h"
#include "Edition.h"
#include "MainInventory.h"
#include "SubInventory.h"
#include "Inventory_Back.h"
#include "Item.h"
#include "CheckBox.h"
#include "PlayerHP_Back.h"
#include "PlayerHP.h"
#include "PlayerHunger_Back.h"
#include "PlayerHunger.h"
#include "PlayerExp_Back.h"
#include "PlayerExp.h"
#include "PlayerLevel.h"
#include "Mouse_Item.h"
#include "ItemFont.h"

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

	#pragma region Texture UI

	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	/* For.Prototype_Component_Texture_BackGround*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_MainLogo*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_MainLogo"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/MainLogo/MinecraftBanner.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_StartButton*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_StartButton"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/StartButton/StartButton1.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_title*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_title"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/title/minecraft.png"), 1))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_edition*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_edition"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/title/edition.png"), 1))))
		return E_FAIL;

	#pragma endregion 

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩딩중입니다."));

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩딩중입니다."));
	
	#pragma region Object UI 
	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩딩중입니다."));

	/* For.Prototype_GameObject_MainLogo */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_MainLogo"),
		CMainLogo::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_StartButton */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_StartButton"),
		CStartButton::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_title */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_title"),
		CTitle::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Edition */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_Edition"),
		CEdition::Create(m_pGraphic_Device))))
		return E_FAIL;

	#pragma endregion 

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

	// 철검 택스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Texture_Sword"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Sword/iron_sword.png"), 1))))
		return E_FAIL;

	// 돌 곡괭이 택스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Texture_Pickaxe"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Pickaxe/stone_pickaxe.png"), 1))))
		return E_FAIL;

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



	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_YUPlay()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));

	// 깨지는 블럭 텍스쳐.
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, PROTOTYPE_COMPONENT_TEXTURE_DESTROY,
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Effect/destroy_stage_%d.png"), 10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sun */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Sun"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/environment/sun.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clouds */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Clouds"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/environment/clouds.png"), 1))))
		return E_FAIL;

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

	/* For.Prototype_Component_Texture_CobbleStone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_CobbleStone"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/cobbleStone%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CoalOre */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_CoalOre"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/coalOre%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Coal */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Coal"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/coal%d.png"), 1))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_IronOre */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_IronOre"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/ironOre%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_RawIron */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_RawIron"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/rawIron%d.png"), 1))))
		return E_FAIL;

	// 나무 텍스쳐
/* For.Prototype_Component_Texture_Leaf */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Leaf"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/leaves0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sapling */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Sapling"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/oakSapling%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Apple */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Apple"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/apple%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Wood"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/wood0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Grass"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/grass0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Seed */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Seed"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/seed%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_RedTulip */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_RedTulip"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/redTulip0.png"), 1))))
		return E_FAIL;

	// 1인칭 Rect_Texture
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Rect_Model"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Rect_model/Rect_model_%d.png"), 2))))
		return E_FAIL;

	// 1인칭 Cube_Texture
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Cube_Model"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Cube_model/Cube_model_%d.png"), 7))))
		return E_FAIL;

	/* UI Component */
	/*================================================================================================*/
	///* For.Prototype_Component_Texture_Inventory*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Inventory"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/inventory/inventory_%03d.png"), 2))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_InventoryBack*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_InventoryBack"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/inventory/Inventory_Back_%03d.png"), 2))))
		return E_FAIL;

	///* For.Prototype_Component_Texture_CheckBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_CheckBox"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/inventory/CheckBox.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Item*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Item"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/Item/Item_%03d.png"), 5))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_PlayerHP*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_PlayerHP"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/PlayerState/Hp_%03d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_PlayerHunger*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_PlayerHunger"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/PlayerState/Hunger_%03d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_PlayerHunger*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_PlayerExp"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/PlayerState/Exp_%03d.png"), 6))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_PlayerLevel*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Font"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/UI/font/font_%03d.png"), 10))))
		return E_FAIL;
	/*================================================================================================*/

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	Engine::CUBE cube{ _float2(64.f, 32.f), _float3(16.f, 16.f, 16.f), _float2(0.f, 0.f) };
	/* For.Prototype_Component_VIBuffer_CubeInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_CubeInstance"),
		CVIBuffer_CubeInstance::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect3DInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Rect3DInstance"),
		CVIBuffer_Rect3DInstance::Create(m_pGraphic_Device))))
		return E_FAIL;


	// 깨지는 블럭 모델.
	Engine::CUBE_ONLY cubeOnly{ _float3(1.1f, 1.1f, 1.1f) };
	/* For.Prototype_Component_VIBuffer_RectShader */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_RectShader"),
		CVIBuffer_RectShader::Create(m_pGraphic_Device))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, PROTOTYPE_COMPONENT_VIBUFFER_ONLY,
		CVIBuffer_Cube_Only::Create(m_pGraphic_Device, cubeOnly))))
		return E_FAIL;

#pragma region 크리퍼

	/* For.Prototype_GameObject_Creeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Creeper"),
		CCreeper::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Creeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Creeper"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Creeper/Creeper.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Creeper */
	cube = { _float2(64.f, 32.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Creeper_Head"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 32.f), _float3(6.f, 12.f, 4.f), _float2(16.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Creeper_Body"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 32.f), _float3(4.f, 6.f, 4.f), _float2(0.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;
#pragma endregion


#pragma region 좀비 텍스처, 모델, 원형객체
	//텍스처
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Zombi"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Zombi/zombie.png"), 1))))
		return E_FAIL;

	// 모델
	/* For.Prototype_Component_VIBuffer_Zombi */
	cube = { _float2(64.f, 64.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Zombi_Head"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 64.f), _float3(8.f, 12.f, 4.f), _float2(16.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Zombi_Body"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	// 다리
	cube = { _float2(64.f, 64.f), _float3(4.f, 12.f, 4.f), _float2(0.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Zombi_Leg"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	// 팔
	cube = { _float2(64.f, 64.f), _float3(4.f, 12.f, 4.f), _float2(40.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Zombi_Arm"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	// 원형 객체
	/* For.Prototype_GameObject_Zombi */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Zombi"),
		CZombi::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, PROTOTYPE_GAMEOBJECT_DESTROY_CUBE,
		CDestroyCube::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion


	/* For.Prototype_Component_VIBuffer_CubeShader */
 	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_CubeShader"),
		CVIBuffer_CubeShader::Create(m_pGraphic_Device, cube))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* UI GameObject */
	/*================================================================================================*/
	/* For.Prototype_GameObject_MainInventory */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_MainInventory"),
		CMainInventory::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_SubInventory*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_SubInventory"),
		CSubInventory::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* For.Prototype_GameObject_SubInventoryBack */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_InventoryBack"),
		CInventory_Back::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_CheckBox */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_CheckBox"),
		CCheckBox::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Item*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Item"),
		CItem::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_PlayerHP_Back*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_PlayerHP_Back"),
		CPlayerHP_Back::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_PlayerHP*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_PlayerHP"),
		CPlayerHP::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_PlayerHunger_Back*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_PlayerHunger_Back"),
		CPlayerHunger_Back::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_PlayerHunger*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_PlayerHunger"),
		CPlayerHunger::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_PlayerExp_Back*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_PlayerExp_Back"),
		CPlayerExp_Back::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_PlayerExp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_PlayerExp"),
		CPlayerExp::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_PlayerExp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_PlayerLevel"),
		CPlayerLevel::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mouse_item*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Mouse_item"),
		CMouse_Item::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Mouse_item*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_ItemFont"),
		CItemFont::Create(m_pGraphic_Device))))
		return E_FAIL;
	/*================================================================================================*/

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

	/* For.Prototype_GameObject_Camera_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Wood"),
		CWood::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Leaf */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Leaf"),
		CLeaf::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tree */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Grass"),
		CGrass::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_RedTulip */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_RedTulip"),
		CRedTulip::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* For.Prototype_GameObject_Sun */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Sun"),
		CSun::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Clouds */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Clouds"),
		CClouds::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_SkyBox */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_SkyBox"),
		CSkyBox::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_ItemCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"),
		CItemCube::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_ItemRect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"),
		CItemRect::Create(m_pGraphic_Device))))
		return E_FAIL;

	//===============================================================================================
		// 스티브 게임 오브젝트
	/* For.Prototype_GameObject_Steve */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Steve"),
		CSteve::Create(m_pGraphic_Device))))
		return E_FAIL;

	// 스티브_팔 게임 오브젝트
	/* For.Prototype_GameObject_Arm_Steve */
 	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_TPS_Arm"),
		CArm_Steve::Create(m_pGraphic_Device))))
		return E_FAIL;

	//==================================<스티브 오른손 물건임>================================================
	/* For.Prototype_GameObject_Rect_Model */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Rect_Model"),
		CRect_Model::Create(m_pGraphic_Device))))
		return E_FAIL;
	/* For.Prototype_GameObject_Cube_Model */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Cube_Model"),
		CCube_Model::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Right_hand */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_RightHand"),
		CRight_hand::Create(m_pGraphic_Device))))
		return E_FAIL;
	//=============================================================================================================
	


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

	/* For.Prototype_Component_Texture_Sun */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_Texture_Sun"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/environment/sun.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Clouds */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_Texture_Clouds"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/environment/clouds.png"), 1))))
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

#pragma region 크리퍼
	/*-----------------------------------
	*  크리퍼 모델
	-----------------------------*/

	/* For.Prototype_GameObject_Creeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_Creeper"),
		CCreeper::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Creeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_Texture_Creeper"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Creeper/Creeper.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Creeper */
	Engine::CUBE cube = { _float2(64.f, 32.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Creeper_Head"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 32.f), _float3(6.f, 12.f, 4.f), _float2(16.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Creeper_Body"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 32.f), _float3(4.f, 6.f, 4.f), _float2(0.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;
#pragma endregion


#pragma region 좀비 텍스처, 모델, 원형객체
	//텍스처
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_Texture_Zombi"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Model_Texture/Zombi/zombie.png"), 1))))
		return E_FAIL;

	// 모델
	/* For.Prototype_Component_VIBuffer_Zombi */
	cube = { _float2(64.f, 64.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Zombi_Head"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	cube = { _float2(64.f, 64.f), _float3(8.f, 12.f, 4.f), _float2(16.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Zombi_Body"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	// 다리
	cube = { _float2(64.f, 64.f), _float3(4.f, 12.f, 4.f), _float2(0.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Zombi_Leg"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	// 팔
	cube = { _float2(64.f, 64.f), _float3(4.f, 12.f, 4.f), _float2(40.f, 16.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Zombi_Arm"),
		CVIBuffer_Anim_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	// 원형 객체
	/* For.Prototype_GameObject_Zombi */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_Zombi"),
		CZombi::Create(m_pGraphic_Device))))
		return E_FAIL;

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));

#pragma region PROTOTYPE
	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));

	/* For.Prototype_GameObject_Sun */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_Sun"),
		CSun::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Clouds */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_Clouds"),
		CClouds::Create(m_pGraphic_Device))))
		return E_FAIL;

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
#pragma endregion
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
#pragma endregion 

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_HECKPlay()
{
	lstrcpy(m_szLoadingText, TEXT(" 텍스쳐을(를) 로딩중입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HYEOK, PROTOTYPE_COMPONENT_TEXTURE_DESTROY,
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Effect/destroy_stage_%d.png"), 10))))
		return E_FAIL;

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

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_TOOL()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐을(를) 로딩중입니다."));
	/* For.Prototype_Component_Texture_Dirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Dirt"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/dirt%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GrassDirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_GrassDirt"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/grassDirt%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Stone"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/stone%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CobbleStone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_CobbleStone"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/cobbleStone%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_CoalOre */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_CoalOre"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/coalOre%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Coal */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Coal"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/coal%d.png"), 1))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_IronOre */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_IronOre"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/ironOre%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_RawIron */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_RawIron"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/rawIron%d.png"), 1))))
		return E_FAIL;

	// 나무 텍스쳐
/* For.Prototype_Component_Texture_Leaf */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Leaf"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/leaves0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sapling */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Sapling"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/oakSapling%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Apple */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Apple"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/apple%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Wood"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/wood0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Grass"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/grass0.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Seed */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Seed"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/seed%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_RedTulip */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_Texture_RedTulip"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/redTulip0.png"), 1))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	Engine::CUBE cube{ _float2(64.f, 32.f), _float3(16.f, 16.f, 16.f), _float2(0.f, 0.f) };
	/* For.Prototype_Component_VIBuffer_CubeInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_VIBuffer_CubeInstance"),
		CVIBuffer_CubeInstance::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect3DInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_Component_VIBuffer_Rect3DInstance"),
		CVIBuffer_Rect3DInstance::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("원형객체을(를) 로딩중입니다."));
	/* For.Prototype_GameObject_Camera_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tool */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_MapTool"),
		CMapTool::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Dirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_Dirt"),
		CDirt::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_GrassDirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_GrassDirt"),
		CGrassDirt::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Stone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_Stone"),
		CStone::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_CoalOre */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_CoalOre"),
		CCoalOre::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_IronOre */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_IronOre"),
		CIronOre::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* For.Prototype_GameObject_Wood */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_Wood"),
		CWood::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Leaf */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_Leaf"),
		CLeaf::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tree */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_Tree"),
		CTree::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Grass */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_Grass"),
		CGrass::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_RedTulip */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, TEXT("Prototype_GameObject_RedTulip"),
		CRedTulip::Create(m_pGraphic_Device))))
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
