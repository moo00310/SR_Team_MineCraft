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

CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* �ε����� ��ü�� �ʿ��� ��ü�� �����Ѵ�. */
	/* ���, �ε���, ��ư, font */

	/* �ε��� ����(���������� �ʿ��� �ڿ�(Resource)(�ؽ���, ��, ���� ���� )�� �����ϴ�)�� 
	������ �δ���ü�� �����Ѵ�. */
	m_pLoader = CLoader::Create(m_pGraphic_Device, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;
	
	return S_OK;
}


void CLevel_Loading::Update(_float fTimeDelta)
{
	

	if (GetKeyState(VK_SPACE) & 0x8000)
	{ 
	
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

			}

			if (nullptr == pLevel)
				return;

			if (FAILED(m_pGameInstance->Change_Level(m_eNextLevelID, pLevel)))
				return;		
		}
	}	
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Output_LoadingText();

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
