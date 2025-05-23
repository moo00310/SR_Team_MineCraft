#include "Level_Loading.h"


#include "Level_GamePlay.h"
#include "Level_Logo.h"
#include "Loader.h"

#include "Level_Moo.h"
#include "Level_Hero.h"
#include "Level_Woo.h"
#include "Level_YU.h"
#include "Level_Hyeok.h"
#include "Level_Tool.h"

#include "GameInstance.h"
#include "LoadingScene.h"

#include "Level_Ending.h"

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	/* For.Prototype_GameObject_LoadingScene*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOADING, TEXT("Prototype_GameObject_LoadingScene"),
		CLoadingScene::Create(m_pGraphic_Device))))
		return E_FAIL;

	if (FAILED(Ready_Layer_LoadingScene(TEXT("Layer_Loadingscene"))))
		return E_FAIL;

	m_eNextLevelID = eNextLevelID;

	/* 로딩레벨 자체에 필요한 객체를 생성한다. */
	/* 배경, 로딩바, 버튼, font */

	/* 로딩의 역할(다음레벨에 필요한 자원(Resource)(텍스쳐, 모델, 사운드 등등등 )을 생성하는)을 
	수행할 로더객체를 생성한다. */
	m_pLoader = CLoader::Create(m_pGraphic_Device, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	CLoadingScene* pLoadingScene = dynamic_cast<CLoadingScene*>(pGameInstance->Get_Object(LEVEL_LOADING, TEXT("Layer_Loadingscene"), 0));

	if (!pLoadingScene)
		return E_FAIL;

	pLoadingScene->Set_Loader(m_pLoader);

	return S_OK;
}


void CLevel_Loading::Update(_float fTimeDelta)
{
	

	//if (GetKeyState(VK_SPACE) & 0x8000)
	//{ 
	
		if (true == m_pLoader->isFinished())
		{
			CLevel* pLevel = { nullptr };

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pGraphic_Device);
				break;
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pGraphic_Device);
				break;

			case LEVEL_MOO:
				pLevel = CLevel_Moo::Create(m_pGraphic_Device);
				break;

			case LEVEL_HERO:
				pLevel = CLevel_HERO::Create(m_pGraphic_Device);
				break;

			case LEVEL_WOO:
				pLevel = CLevel_WOO::Create(m_pGraphic_Device);
				break;

			case LEVEL_YU:
				pLevel = CLevel_YU::Create(m_pGraphic_Device);
				break;

			case LEVEL_HYEOK:
				pLevel = CLevel_Hyeok::Create(m_pGraphic_Device);
				break;

			case LEVEL_TOOL:
				pLevel = CLevel_TOOL::Create(m_pGraphic_Device);
				break;

			case LEVEL_ENDING:
				pLevel = CLevel_Ending::Create(m_pGraphic_Device);
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(m_pGameInstance->Change_Level(m_eNextLevelID, pLevel)))
				return;		
		}
	//}	
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Output_LoadingText();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_LoadingScene(const _wstring& strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	/* Prototype_GameObject_MainInventory */
	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, TEXT("Prototype_GameObject_LoadingScene"),
		LEVEL_LOADING, strLayerTag)))
		return E_FAIL;	

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
