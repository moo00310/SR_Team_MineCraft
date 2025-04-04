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
}

void CGameMgr::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down('P'))
	{
		Spwan_Monster();
	}

}

void CGameMgr::Late_Update(_float fTimeDelta)
{


}

void CGameMgr::Spwan_Monster()
{
	// 몬스터 스폰가능한 위치 들고옴
	m_SpawnPos = m_pTerrain->Get_SpwanAble();

	//for (_uint i = 0; i < 1; i++)
	//{
	//	CGameObject* ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Creeper"),
	//		LEVEL_YU, TEXT("Layer_Monster"));

	//	static_cast<CMonster*>(ptemp)->Reset_Monster();
	//	static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[9]));
	//}

	for (_uint i = 0; i < 1; i++)
	{
		CGameObject* ptemp = m_pGameInstance->PushPool(LEVEL_YU, TEXT("Prototype_GameObject_Zombi"),
			LEVEL_YU, TEXT("Layer_Monster"));

		static_cast<CMonster*>(ptemp)->Reset_Monster();
		static_cast<CMonster*>(ptemp)->Get_Transform()->Set_State(CTransform::STATE_POSITION, _float3(m_SpawnPos[11]));
		
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
