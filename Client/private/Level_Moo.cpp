#include "Level_Moo.h"
#include "GameInstance.h"
#include "Tree.h"
#include "Item_Model.h"

CLevel_Moo::CLevel_Moo(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel{ pGraphic_Device }
{
}

HRESULT CLevel_Moo::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Creeper(TEXT("Layer_Creeper"))))
	//	return E_FAIL;

	//if (FAILED(Ready_Layer_Steve(TEXT("Layer_Steve"))))
	//	return E_FAIL;

	//임무결 1인칭 확인용
	if (FAILED(Ready_Laye_MooArm(TEXT("Layer_Arm"))))
		return E_FAIL;

	//if (FAILED(Ready_Laye_Sword(TEXT("Layer_Item"))))
	//	return E_FAIL;

#ifdef _DEBUG

	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

	// :one: 콘솔 핸들 가져오기
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// :two: 콘솔 버퍼 크기 먼저 조정
	COORD bufferSize = { 20, 20 }; // 가로 80, 세로 20
	SetConsoleScreenBufferSize(hConsole, bufferSize);

	// :three: 콘솔 창 크기 조정
	SMALL_RECT windowSize = { 0, 0, 80 - 1, 20 - 1 };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

#endif // _DEBUG

	return S_OK;
}

void CLevel_Moo::Update(_float fTimeDelta)
{
	
}

HRESULT CLevel_Moo::Render()
{
	SetWindowText(g_hWnd, TEXT("MOO_TEST"));

	return S_OK;
}

HRESULT CLevel_Moo::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_MOO, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Moo::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_MOO, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Moo::Ready_Layer_Creeper(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Creeper"),
		LEVEL_MOO, strLayerTag)))
			return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Moo::Ready_Layer_Steve(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_Steve"),
		LEVEL_MOO, strLayerTag)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Moo::Ready_Laye_MooArm(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, TEXT("Prototype_GameObject_TPS_Arm"),
		LEVEL_STATIC, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Moo::Ready_Laye_Sword(const _wstring& strLayerTag)
{
	CItem_Model::DESC desc
	{
		TEXT("Prototype_Component_Texture_Pickaxe"),
		LEVEL_MOO
	};

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MOO, TEXT("Prototype_GameObject_Sword"),
		LEVEL_MOO, strLayerTag, &desc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Moo* CLevel_Moo::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Moo* pInstance = new CLevel_Moo(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Moo");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CLevel_Moo::Free()
{
	__super::Free();

}
