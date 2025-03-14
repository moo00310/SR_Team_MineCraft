#include "Loader.h"

#include "GameInstance.h"

#include "Camera_Free.h"
#include "BackGround.h"
#include "Terrain.h"
#include "player.h"

#include "Tool.h"

/*
* ũ���� �׽�Ʈ ��
*/
#include "Creeper.h"
#include "Creeper_Body.h"
#include "Creeper_Head.h"
#include "Creeper_Foot.h"

//HERO
#include "HeroCube.h" //�ݶ��̴� �׽�Ʈ�� ť��
/*
  ���� ����
*/
#include "Dirt.h"
#include "Stone.h"
#include "GrassDirt.h"
#include "MCTerrain.h"
#include "MapTool.h"



CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device { pGraphic_Device }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	/* �ڿ��ε��Ѵ�. */
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

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	
	lstrcpy(m_szLoadingText, TEXT("�ؽ�����(��) �ε����Դϴ�."));
	/* For.Prototype_Component_Texture_BackGround*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("����(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������(��) �ε����Դϴ�."));
	

	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε����Դϴ�."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pGraphic_Device))))
		return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ�����(��) �ε����Դϴ�."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("����(��) �ε����Դϴ�."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 256, 256))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("������(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε����Դϴ�."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_MOOPlay()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ�����(��) �ε����Դϴ�."));
	/* For.Prototype_Component_Texture_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Creeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Texture_Creeper"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/Textures/Creeper/Creeper.png"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("����(��) �ε����Դϴ�."));
	/* For.Prototype_Component_VIBuffer_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pGraphic_Device, 256, 256))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube */
	// head : _float2(64.f, 32.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f)
	// body : _float2(64.f, 32.f), _float3(6.f, 12.f, 4.f), _float2(16.f, 16.f)
	// foot : _float2(64.f, 32.f), _float3(4.f, 6.f, 4.f), _float2(0.f, 16.f)

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


	//// �ͷ��� ť��
	//cube = { _float2(64.f, 32.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f) };
	///* For.Prototype_Component_Com_mVIBCubeTerrain */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_Component_Com_mVIBCubeTerrain"),
	//	CVIBuffer_CubeTerrain::Create(m_pGraphic_Device, cube))))
	//	return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("������(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε����Դϴ�."));
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


	/* For.Prototype_GameObject_Head */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper_Head"),
		CCreeper_Head::Create(m_pGraphic_Device))))
		return E_FAIL;


	/* For.Prototype_GameObject_Body */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper_Body"),
		CCreeper_Body::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Creeper */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper_Foot"),
		CCreeper_Foot::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_YUPlay()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ�����(��) �ε����Դϴ�."));
	/* For.Prototype_Component_Texture_Dirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Dirt"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/dirt%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_GrassDirt */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_GrassDirt"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/grassdirt%d.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Stone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_Texture_Stone"),
		CTexture::Create(m_pGraphic_Device, TEXT("../Bin/Resources/MCTextures/stone%d.png"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("����(��) �ε����Դϴ�."));
	Engine::CUBE cube{ _float2(64.f, 32.f), _float3(16.f, 16.f, 16.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_BreakableCube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device, cube))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("������(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε����Դϴ�."));
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Dirt"),
		CDirt::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_Stone"),
		CStone::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_GrassDirt"),
		CGrassDirt::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_MCTerrain"),
		CMCTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Tool */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_YU, TEXT("Prototype_GameObject_MapTool"),
		CMapTool::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_HEROPlay()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ�����(��) �ε����Դϴ�."));

	lstrcpy(m_szLoadingText, TEXT("�ݶ��̴���(��) �ε����Դϴ�."));
	/* For.Prototype_Component_CCollider_Cube */
	CCollider_Cube::COLLRECTDESC Desc{};
	Desc.fRadiusX = 1.f; Desc.fRadiusY = 1.f; Desc.fRadiusZ = 1.f; //�ݶ��̴� ũ�� ����(�� �̰Ÿ� �ϸ� ������ ������ ���°�)
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_CCollider_Cube"),
		CCollider_Cube::Create(m_pGraphic_Device, Desc))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("����(��) �ε����Դϴ�."));
	/* For.Prototype_Component_VIBuffer_Cube */
	CUBE tCube{ _float2(64.f, 32.f), _float3(8.f, 8.f, 8.f), _float2(0.f, 0.f) };
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pGraphic_Device, tCube))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("������(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε����Դϴ�."));
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pGraphic_Device))))
		return E_FAIL;

	/* For.Prototype_GameObject_HeroCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HERO, TEXT("Prototype_GameObject_HeroCube"),
		CHeroCube::Create(m_pGraphic_Device))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_WOOPlay()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ�����(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("����(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε����Դϴ�."));
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

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_HECKPlay()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ�����(��) �ε����Դϴ�."));
	

	lstrcpy(m_szLoadingText, TEXT("����(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε����Դϴ�."));
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

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_TOOL()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ�����(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("����(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������ü��(��) �ε����Դϴ�."));
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

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

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
