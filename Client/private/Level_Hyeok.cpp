#include "Level_Hyeok.h"
#include "GameInstance.h"

CLevel_Hyeok::CLevel_Hyeok(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel { pGraphic_Device }
{

}

HRESULT CLevel_Hyeok::Initialize()
{
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	/*if (FAILED(Ready_Layer_Hyock(TEXT("Layer_Hyock_Cube"))))
		return E_FAIL;*/

	/*if (FAILED(Ready_Layer_TestParticle(TEXT("Layer_TestParticle"))))
		return E_FAIL;*/	

	/*if (FAILED(Ready_Layer_DigParticle(TEXT("Layer_ParticleDig"))))
		return E_FAIL;*/	

	if (FAILED(Ready_Layer_Particle(LAYER_PARTICLE)))
		return E_FAIL;

	return S_OK;
}

void CLevel_Hyeok::Update(_float fTimeDelta)
{
	int a = 10;
}

HRESULT CLevel_Hyeok::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	SetWindowText(g_hWnd, TEXT("김동혁 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_Hyeok::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HYEOK, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_HYEOK, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hyeok::Ready_Layer_Player(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 1; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HYEOK, TEXT("Prototype_GameObject_Player"),
			LEVEL_HYEOK, strLayerTag)))
			return E_FAIL;
	}
	

	return S_OK;
}
HRESULT CLevel_Hyeok::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HYEOK, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_HYEOK, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hyeok::Ready_Layer_Hyock(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HYEOK, TEXT("Prototype_GameObject_Hyock_Cube"),
		LEVEL_HYEOK, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hyeok::Ready_Layer_TestParticle(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HYEOK, TEXT("Prototype_GameObject_TestParticle"),
		LEVEL_HYEOK, strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Hyeok::Ready_Layer_DigParticle(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_HYEOK, TEXT("Prototype_GameObject_ParticleDig"),
		LEVEL_HYEOK, strLayerTag)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CLevel_Hyeok::Ready_Layer_Particle(const _wstring& strLayerTag)
{
	// 비 파티클.
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STATIC, PROTOTYPE_GAMEOBJECT_PARTICLE_RAIN,
		LEVEL_HYEOK, strLayerTag)))
		return E_FAIL;

	// 달리기 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,	// 가져올 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_DASH,	// 가져올 프로토타입.
		LEVEL_HYEOK,	// 적용 씬..
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 흙 파괴 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,	// 가져올 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_DESTROY,	// 가져올 프로토타입.
		LEVEL_HYEOK,	// 적용 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 폭발 파티클.	
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,	// 가져올 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_EXPLOSION,	// 가져올 프로토타입.
		LEVEL_HYEOK,	// 적용 씬.
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

	// 불꽃 검 파티클.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,	// 가져올 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME,			// 가져올 프로토타입.
		LEVEL_STATIC,		// 적용 씬.
		strLayerTag,		// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}	

	CParticleEventManager::Get_Instance()->OnParticle(
		PROTOTYPE_GAMEOBJECT_PARTICLE_SWORD_FLAME,
		{ 0.f, 0.f, 0.f }
	);

	// 폭죽
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_FIRE_CRACKER,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	// 차징.
	if (FAILED(m_pGameInstance->CreatePool(LEVEL_STATIC,		// 적용 씬.
		PROTOTYPE_GAMEOBJECT_PARTICLE_GLITTER,	// 가져올 프로토타입.
		LEVEL_STATIC,	// 가져올 씬.
		strLayerTag,	// 애드오브젝트에 추가할 레이어.
		3)))				// 풀링 갯수.
	{
		return E_FAIL;
	}

	return S_OK;
}

CLevel_Hyeok* CLevel_Hyeok::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLevel_Hyeok* pInstance = new CLevel_Hyeok(pGraphic_Device);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Hyeok");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Hyeok::Free()
{
	__super::Free();

}
