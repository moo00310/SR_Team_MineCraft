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

	m_fHardness = 0.2f;

	return S_OK;
}

void CLeaf::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CLeaf::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CLeaf::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CLeaf::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	m_pTransformCom->Bind_Resource(m_pShaderCom);
	m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);

	m_pShaderCom->Begin(0);

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();

	__super::Render();

	return S_OK;
}

HRESULT CLeaf::Drop_Item_OnDestroy(const _float3& fPos)
{
	int random = rand() % 100;

	if (random < 10 || random < 20) // 10% 확률 사과 or 묘목
	{
		wchar_t layerName[100];
		swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
			return E_FAIL;

		if (CItemRect* pItem = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)))
		{
			pItem->SetPos(fPos);

			if (random < 10)
				pItem->Set_ItemTypeAndBindTexture(ITEMNAME_SAPLING);
			else
				pItem->Set_ItemTypeAndBindTexture(ITEMNAME_APPLE);
		}
	}

	return S_OK;
}
HRESULT CLeaf::Play_Destroy_Effect(const _float3& fPos)
{
	//m_pGameInstance->Check_Sound_Stop(this, 0, SOUND_BLOCK);
	m_pGameInstance->Play_Sound(TEXT("Grass_dig1"), SOUND_BLOCK_DIG, this, 1.f, fPos);

	return S_OK;
}

void CLeaf::PlaySound_Hit(_float3 vPos)
{
	m_pGameInstance->Play_Sound(TEXT("Grass_hit1"), SOUND_BLOCK_HIT, this, 1.f, vPos);
}

void CLeaf::RemoveLeaf()
{
	int random = rand() % m_vecPositions.size();

	Delete_Cube(m_vecPositions[random]);
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
	__super::Free();
}

