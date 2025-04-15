#include "Level_YU.h"
#include "GameInstance.h"
#include "MapTool.h"
#include "Camera_Player.h"
#include "Steve.h"
#include "SlotInfo.h"
#include "UI_Mgr.h"
#include "Pawn.h"
#include "Sun.h"
#include "Camera_Cutscene.h"
#include "Level_Loading.h"

CLevel_YU::CLevel_YU(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_YU::Initialize()
{
#ifdef _DEBUG

	//if (::AllocConsole() == TRUE)
	//{
	//	FILE* nfp[3];
	//	freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
	//	freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
	//	freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
	//	std::ios::sync_with_stdio();
	//}
	//
	//// :one: 콘솔 핸들 가져오기
	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//
	//// :two: 콘솔 버퍼 크기 먼저 조정
	//COORD bufferSize = { 20, 20 }; // 가로 80, 세로 20
	//SetConsoleScreenBufferSize(hConsole, bufferSize);
	//
	//// :three: 콘솔 창 크기 조정
	//SMALL_RECT windowSize = { 0, 0, 80 - 1, 20 - 1 };
	//SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

#endif // _DEBUG

	if (FAILED(Ready_Layer_DestroyCube(LAYER_DESTROY_CUBE)))
		return E_FAIL;

	if (FAILED(Ready_Layer_HitCube(TEXT("Layer_HitLayCube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Steve(TEXT("Layer_Steve"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_SkyBox(TEXT("Layer_SkyBox"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Clouds(TEXT("Layer_Clouds"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Sun(TEXT("Layer_Sun"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_GameMgr(TEXT("Ready_Layer_GameMgr"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Crosshair(TEXT("Layer_Crosshair"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Inventory(TEXT("Layer_Inventory"))))
		return E_FAIL;

	if(FAILED(Ready_Layer_PlayerState(TEXT("Layer_PlayerState"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Effect(LAYER_EFFECT)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Particle(LAYER_PARTICLE)))
		return E_FAIL;

	if (FAILED(Ready_Laye_Creeper(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Laye_Zombi(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Laye_Warden(TEXT("Layer_Monster"))))
		return E_FAIL;

	///// 오른손 객체들과 그걸 관리할 오브젝트
	if (FAILED(Ready_Layer_TPS_Arm(TEXT("Layer_RightHand"))))
		return E_FAIL;

	if (FAILED(Ready_Laye_Rect_Model(TEXT("Layer_RightHand"))))
		return E_FAIL;

	if (FAILED(Ready_Laye_Cube_Model(TEXT("Layer_RightHand"))))
		return E_FAIL;

	if (FAILED(Ready_Laye_Left_Rect_Model(TEXT("Layer_RightHand"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI_DropItem(LAYER_UI_POOL)))
		return E_FAIL;

	// 위 객체들을 관리하는 오브젝트
	if (FAILED(Ready_Laye_RightHand(TEXT("Layer_RightHand_GameObject"))))
		return E_FAIL;	

	///////////////////////////////////////////////////////////	

	if (FAILED(Ready_Layer_Missions(TEXT("Layer_Mission"))))
		return E_FAIL;

	///////////////////////////////////////////////////////////

	if (FAILED(Ready_Layer_Camera_Cutscene(TEXT("Layer_Camera_Cutscene"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BossUI(TEXT("Layer_BossUI"))))
		return E_FAIL;

	m_pGameInstance->PlayBGM(L"sweden");

	return S_OK;
}

void CLevel_YU::Update(_float fTimeDelta)
{
	ftime += fTimeDelta;
	m_iFPS++;
	 
	CUI_Mgr::Get_Instance()->Update(fTimeDelta);
	CUI_Mgr::Get_Instance()->Late_Update(fTimeDelta);

	//if (m_pGameInstance->Key_Down(VK_ADD))
	//{
	//	static_cast<Camera_Cutscene*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Camera_Cutscene")))->Start_Cutscene({ 10.f, 10.f, 10.f });
	//}

	//if (m_pGameInstance->Key_Down(VK_SUBTRACT))
	//{
	//	static_cast<Camera_Cutscene*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Camera_Cutscene")))->End_Cutscene();
	//}

	if (m_pGameInstance->Key_Down(VK_ADD))
	{
		if (m_pGameInstance->Change_Level(LEVEL_LOADING,
			CLevel_Loading::Create(m_pGraphic_Device, LEVEL_ENDING)))
			return;
	}
}

HRESULT CLevel_YU::Render()
{
	if (ftime >= 1.f)
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);
		ftime = 0;
		m_iFPS = 0;
	}
	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Terrain(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_MCTerrain"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_GameMgr(const _wstring& strLayerTag)
{
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_GameMgr"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Tool(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_MapTool"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Steve(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Steve"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	dynamic_cast<CSteve*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Steve"), 0))->SetPos(_float3(50.f, 20.f, 50.f));

	CUI_Mgr::Get_Instance()->Set_Steve(dynamic_cast<CSteve*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Steve"), 0)));

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Sun(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Sun"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Clouds(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Clouds"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_SkyBox(const _wstring& strLayerTag)
{
	/* Prototype_GameObject_SkyBox */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_SkyBox"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Inventory(const _wstring& strLayerTag)
{
	/* Prototype_GameObject_MainInventory */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_MainInventory"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	/* Prototype_GameObject_SubInventory */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_SubInventory"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	/* Prototype_GameObject_FurnaceUi */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_FurnaceUi"),
		LEVEL_YU, TEXT("Layer_FurnaceUi"))))
		return E_FAIL;

	/* Prototype_GameObject_CraftingTable */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_CraftingTable"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	/* Prototype_GameObject_Inventory_Bag */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Inventory_Bag"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	/* Prototype_GameObject_InventoryBack */
	for (int i = 0; i < 59; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_InventoryBack"),
			LEVEL_YU, strLayerTag, (int*)&i)))
			return E_FAIL;
	}

	/* Prototype_GameObject_Item */
	for (int i = 0; i < 69; ++i) // 58
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Item"),
			LEVEL_YU, strLayerTag, (int*)&i)))
			return E_FAIL;
	}

	/* Prototype_GameObject_CheckBox */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_CheckBox"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	/* Prototype_GameObject_Mouse_item */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Mouse_item"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	/* Prototype_GameObject_Mouse_ItemFont */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Mouse_ItemFont"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;	

	/* Prototype_GameObject_Inventory_Item */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Inventory_Item"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	/* Prototype_GameObject_Crafting */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Crafting"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	/* Prototype_GameObject_BurnUi */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_BurnUi"),
		LEVEL_YU, TEXT("Layer_FurnaceDetailUi"))))
		return E_FAIL;

	/* Prototype_GameObject_BurnResultUi */
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_BurnResultUi"),
		LEVEL_YU, TEXT("Layer_FurnaceDetailUi"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_PlayerState(const _wstring& strLayerTag)
{
	/* Prototype_GameObject_PlayerHP_Back */
	for (int i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_PlayerHP_Back"),
			LEVEL_YU, strLayerTag, (int*)&i)))
			return E_FAIL;
	}

	/* Prototype_GameObject_PlayerHP */
	for (int i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_PlayerHP"),
			LEVEL_YU, strLayerTag, (int*)&i)))
			return E_FAIL;
	}
	
	/* Prototype_GameObject_PlayerHunger_Back */
	for (int i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_PlayerHunger_Back"),
			LEVEL_YU, strLayerTag, (int*)&i)))
			return E_FAIL;
	}

	/* Prototype_GameObject_PlayerHunger*/
	for (int i = 0; i < 10; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_PlayerHunger"),
			LEVEL_YU, strLayerTag, (int*)&i)))
			return E_FAIL;
	}

	/* Prototype_GameObject_PlayerExp_Back*/
	for (int i = 0; i < 18; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_PlayerExp_Back"),
			LEVEL_YU, strLayerTag, (int*)&i)))
			return E_FAIL;
	}

	/* Prototype_GameObject_PlayerExp*/
	for (int i = 0; i < 18; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_PlayerExp"),
			LEVEL_YU, strLayerTag, (int*)&i)))
			return E_FAIL;
	}

	for (int i = 0; i < 2; ++i)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_PlayerLevel"),
			LEVEL_YU, strLayerTag, (int*)&i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_YU::Ready_Laye_Creeper(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_YU, TEXT("Prototype_GameObject_Creeper"),

		LEVEL_YU, strLayerTag, 20)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_YU::Ready_Laye_Zombi(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_YU, TEXT("Prototype_GameObject_Zombi"),
		LEVEL_YU, strLayerTag, 20)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Laye_Warden(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_YU, TEXT("Prototype_GameObject_Warden"),
		LEVEL_YU, strLayerTag, 1)))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Warden"),
	//	LEVEL_YU, strLayerTag)))
	//	return E_FAIL;

	//_float3 vPos = _float3{ 30.f, 10.f, 30.f };
	//CPawn* pPawn = static_cast<CPawn*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Monster")));
	//pPawn->Set_Animation(CPawn::FIND);
	//pPawn->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPos);
	//
	//static_cast<Camera_Cutscene*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Camera_Cutscene")))->Start_Cutscene(vPos);


	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_DestroyCube(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, PROTOTYPE_GAMEOBJECT_DESTROY_CUBE,
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_HitCube(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_LayHitCube"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_UI_DropItem(const _wstring& strLayerTag)
{
	// 드랍 아이템 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_YU,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_UI_DROP_ITEM,	// 가져올 프로토타입.
		LEVEL_YU,		// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))			// 풀링 갯수.
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Missions(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_MissionMainUi"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_WaveUi"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_MissionControl"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Crosshair(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Crosshair"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Camera_Cutscene(const _wstring& strLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Camera_Cutscene"),
	//	LEVEL_YU, strLayerTag)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_BossUI(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_BossUi"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_TPS_Arm(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_TPS_Arm"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Laye_Rect_Model(const _wstring& strLayerTag)
{
	// 렉트모델 게임오브젝트 옮겨야겠다
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Rect_Model"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Laye_Cube_Model(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Cube_Model"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Laye_RightHand(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_RightHand"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Laye_Left_Rect_Model(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Left_Rect_Model"),
		LEVEL_YU, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_YU,	// 적용 씬.
		PROTOTYPE_GAMEOBJECT_SWORD_AURA,	// 가져올 프로토타입.
		LEVEL_YU,		// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))			// 풀링 갯수.
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_YU::Ready_Layer_Particle(const _wstring& strLayerTag)
{	
	// 달리기 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,	// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_DASH,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 사망 파티클.	
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,	// 가져올 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_DIE,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 적용 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 폭발 파티클.	
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,	// 가져올 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_EXPLOSION,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 적용 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 흙 캐는 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_MINING,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 흙 파괴 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_DESTROY,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 돌 파괴 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_DESTROY,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 나뭇잎 파괴 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_LEAF_DESTROY,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 나무 캐는 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 돌 캐는 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 나뭇잎 캐는 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_LEAF_MINING,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 불검 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		20)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 폭죽
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_FIRE_CRACKER,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		5)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 사과 먹는거.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_EATING,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 검기 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_AURA,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		20)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 폭죽 길 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_CRACKER_LOAD,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		5)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 폭죽 길 오브젝트.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_YU,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_CRACKER_LOAD,	// 가져올 프로토타입.
		LEVEL_YU,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		5)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 워든 원거리 공격 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_SONIC_BOOM,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 화로 불 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_FURNACE,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		1)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 화로 구름 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_FURNACE_CLOUD,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		1)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	return S_OK;
}

//HRESULT CLevel_YU::Ready_Layer_Camera(const _wstring& strLayerTag) {
//	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Camera_Free"),
//		LEVEL_YU, strLayerTag)))
//		return E_FAIL;
//
//	return S_OK;
//
//}


HRESULT CLevel_YU::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Player::CAMERA_PLAYER_DESC Cam_Player_Desc{};
	Cam_Player_Desc.vEye = _float3(0.f, 10.f, -10.f);
	Cam_Player_Desc.vAt = _float3(0.f, 0.f, 0.f);
	Cam_Player_Desc.fFov = D3DXToRadian(60.f);
	Cam_Player_Desc.fNear = 0.1f;
	Cam_Player_Desc.fFar = 300.f;
	Cam_Player_Desc.fMouseSensor = 0.1f;
	Cam_Player_Desc.pTarget = m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Steve"), 0);//게임인스턴스-> Find Layer-> Steve Layer에서 GameObject* 가져와야 할 듯
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Camera_Player"),
		LEVEL_YU, strLayerTag, &Cam_Player_Desc)))
		return E_FAIL;

	return S_OK;
}


CLevel_YU* CLevel_YU::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_YU* pInstance = new CLevel_YU(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_YU");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_YU::Free()
{
	__super::Free();

	//CUI_Mgr::Get_Instance()->Free();
	//CUI_Mgr::Get_Instance()->Destroy_Instance();
}
