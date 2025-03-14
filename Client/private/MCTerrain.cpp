#include "MCTerrain.h"
#include "GameInstance.h"

CMCTerrain::CMCTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{

}

CMCTerrain::CMCTerrain(const CMCTerrain& Prototype)
	: CGameObject(Prototype),
	m_iMapX(Prototype.m_iMapX), 
	m_iMapY(Prototype.m_iMapY),
	m_iMapZ(Prototype.m_iMapZ)
{

}

HRESULT CMCTerrain::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMCTerrain::Initialize(void* pArg)
{
    return S_OK;
}

void CMCTerrain::Priority_Update(_float fTimeDelta)
{
}

void CMCTerrain::Update(_float fTimeDelta)
{


}

void CMCTerrain::Late_Update(_float fTimeDelta)
{	
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CMCTerrain::Render()
{

    return S_OK;
}

void CMCTerrain::SetMapSize(int iMapX, int iMapY, int iMapZ)
{
	m_iMapX = iMapX;
	m_iMapY = iMapY;
	m_iMapZ = iMapZ;
}

HRESULT CMCTerrain::TerrainGeneration()
{
	m_pGameInstance->ClearLayer(LEVEL_YU, TEXT("Layer_BackGround"));

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}


HRESULT CMCTerrain::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	//2차원 
	/*
		for (int i = 0; i < m_iMapX; ++i) {
		for (int j = 0; j < m_iMapZ; ++j) {
			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Dirt"),
				LEVEL_YU, strLayerTag)))
				return E_FAIL;

			_float3 temp = { (float)j,0.f, (float)i };
			dynamic_cast<CDirt*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_BackGround"), i * m_iMapZ + j))->SetPos(temp);
		}
	}
	*/


	// 3차원
	for (int i = 0; i < m_iMapY; ++i) {  // Y축이 높이
		for (int j = 0; j < m_iMapX; ++j) {  // X축
			for (int k = 0; k < m_iMapZ; ++k) {  // Z축
				if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Dirt"),
					LEVEL_YU, strLayerTag)))
					return E_FAIL;

				
				_float3 temp = { (float)j,(float)i,(float)k };
				dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_BackGround"), (i * m_iMapX * m_iMapZ) + (j * m_iMapZ) + k))->SetPos(temp);
			}
		}
	}

	return S_OK;
}

CMCTerrain* CMCTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMCTerrain* pInstance = new CMCTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMCTerrain::Clone(void* pArg)
{	
	CMCTerrain* pInstance = new CMCTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMCTerrain::Free()
{
	__super::Free();


}
