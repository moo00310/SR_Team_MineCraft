#include "Leaf.h"

CLeaf::CLeaf(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBreakableCube(pGraphic_Device)
{
}

CLeaf::CLeaf(const CLeaf& Prototype)
	: CBreakableCube(Prototype)
{
}

HRESULT CLeaf::Initialize_Prototype()
{
	return S_OK;;
}

HRESULT CLeaf::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CLeaf::Priority_Update(_float fTimeDelta)
{
}

void CLeaf::Update(_float fTimeDelta)
{
}

void CLeaf::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CLeaf::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::Render();

	return S_OK;
}

HRESULT CLeaf::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Leaf"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	__super::Ready_Components();

	return S_OK;
}

CLeaf* CLeaf::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLeaf* pInstance = new CLeaf(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLeaf::Clone(void* pArg)
{
	CLeaf* pInstance = new CLeaf(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CStone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLeaf::Free()
{
	int random = rand() % 100;
	if (random < 10) {
		wchar_t layerName[100];
		swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
			return;
		dynamic_cast<CTransform*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))->Set_ItemTypeAndBindTexture(ITEM_SAPLING);
	}
	else if (10 <= random && random < 20) {
		wchar_t layerName[100];
		swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
			return;
		dynamic_cast<CTransform*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))->Set_ItemTypeAndBindTexture(ITEM_APPLE);
	}

	__super::Free();
}

