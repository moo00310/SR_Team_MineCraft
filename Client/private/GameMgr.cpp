#include "GameMgr.h"
#include "Monster.h"

CGameMgr::CGameMgr(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CGameMgr::CGameMgr(const CGameMgr& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CGameMgr::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameMgr::Initialize(void* pArg)
{
	m_fSpwanCoolTime = 0.f;

	m_pTerrain = static_cast<CMCTerrain*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Terrain")));
	m_pSun = static_cast<CSun*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Sun")));

	if (!m_pTerrain || !m_pSun)
		return E_FAIL;

	Safe_AddRef(m_pTerrain);
	Safe_AddRef(m_pSun);

	return S_OK;
}

void CGameMgr::Priority_Update(_float fTimeDelta)
{
	// 스폰 쿨 10초가 돌았고 현재 밤이며 활성화된 몬스터 개수가 20보다 작으면 몬스터 스폰
	// 20 마리 유지
	m_fSpwanCoolTime += fTimeDelta;

	//if (m_fSpwanCoolTime >= 10.f && !m_pSun->Get_Sun())
	//{
	//	int spawnCount = 20 - m_pGameInstance->GetActiveCount(TEXT("Layer_Monster"));
	//	if (spawnCount == 0) return;

	//	Spwan_Monster(spawnCount);
	//	m_fSpwanCoolTime = 0.f;
	//}

}

void CGameMgr::Update(_float fTimeDelta)
{
	
}

void CGameMgr::Late_Update(_float fTimeDelta)
{

}

void CGameMgr::Spwan_Monster(int count)
{
	// 몬스터 스폰가능한 위치 들고옴
	m_SpawnPos = m_pTerrain->Get_SpwanAble();

	for (int i = 0; i < count; i++)
	{
		int Random_pos = rand() % m_SpawnPos.size();
		int Random_type = rand() % 2;
		CGameObject* ptemp = nullptr;

		if (Random_type == 0)
		{
			ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Creeper"),
				LEVEL_YU, TEXT("Layer_Monster"));
		}
		else
		{
			ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Zombi"),
				LEVEL_YU, TEXT("Layer_Monster"));
		}

		if (ptemp == nullptr) return;
		static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[Random_pos]));
	}

}

CGameMgr* CGameMgr::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CGameMgr* pInstance = new CGameMgr(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGameMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGameMgr::Clone(void* pArg)
{
	CGameMgr* pInstance = new CGameMgr(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGameMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameMgr::Free()
{
	__super::Free();
	Safe_Release(m_pTerrain);
	Safe_Release(m_pSun);
}
