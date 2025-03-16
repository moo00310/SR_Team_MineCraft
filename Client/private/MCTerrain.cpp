#include "MCTerrain.h"
#include "GameInstance.h"

CMCTerrain::CMCTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{

}

CMCTerrain::CMCTerrain(const CMCTerrain& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CMCTerrain::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMCTerrain::Initialize(void* pArg)
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Environment"))))
		return E_FAIL;

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

HRESULT CMCTerrain::Ready_Layer_BackGround(const _wstring& strLayerTag)
{

	HANDLE hFile = CreateFile(L"../bin/Resources/DataFiles/BlockData.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return S_OK;
	}
	DWORD dwBytesRead;
	BLOCKDESC eblockData;
	int index = 0;

	while (ReadFile(hFile, &eblockData, sizeof(BLOCKDESC), &dwBytesRead, NULL) && dwBytesRead > 0)
	{
		switch (eblockData.eBlockType)
		{
		case GRASSDIRT:
			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_GrassDirt"), LEVEL_YU, strLayerTag)))
				return E_FAIL;
			break;
		case DIRT:
			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Dirt"), LEVEL_YU, strLayerTag)))
				return E_FAIL;
			break;
		case STONE:
			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_Stone"), LEVEL_YU, strLayerTag)))
				return E_FAIL;
			break;
		default:
			break;
		}


		CBreakableCube* pCube = dynamic_cast<CBreakableCube*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Environment"), index));
		if (pCube) {
			pCube->SetPos(_float3(eblockData.fPosition));
		}
		index++;
	}

	CloseHandle(hFile);
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
