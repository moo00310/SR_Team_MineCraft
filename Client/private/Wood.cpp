#include "Wood.h"
#include "MissionControl.h"

CWood::CWood(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CBreakableCube(pGraphic_Device)
{
}

CWood::CWood(const CWood& Prototype)
	: CBreakableCube(Prototype)
{
}

HRESULT CWood::Initialize_Prototype()
{
	return S_OK;;
}

HRESULT CWood::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_itemName = ITEMNAME_WOOD;
	m_fHardness = 2;


	return S_OK;
}

void CWood::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CWood::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CWood::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CWood::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	m_pTransformCom->Bind_Resource(m_pShaderCom);
	m_pTransformCom->Bind_Resource_BlockType(m_pShaderCom, m_itemName);
	m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);

	if (g_bIsScan == false)
	{
		m_pShaderCom->Begin(0);
	}
	else
	{
		m_pShaderCom->Begin(2);
	}

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();

	__super::Render();

	return S_OK;
}

HRESULT CWood::Drop_Item_OnDestroy(const _float3& fPos)
{

	wchar_t layerName[100];
	swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
		return E_FAIL;

	if (CItemCube* pItem = dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)))
	{
		pItem->SetPos(fPos);
		pItem->Set_Bright(fPos.y);
		pItem->Set_ItemTypeAndBindTexture(ITEMNAME_WOOD);
	}

	if (CMissionControl* _control = dynamic_cast<CMissionControl*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Mission")))) {
		_control->Update_Mission(L"나무");
	}

	return S_OK;
}

HRESULT CWood::Play_Destroy_Effect(const _float3& vPos)
{
	//m_pGameInstance->Check_Sound_Stop(this, 0, SOUND_BLOCK);
	m_pGameInstance->Play_Sound(TEXT("Wood_dig"), SOUND_BLOCK_DIG, this, 1.f, vPos);

	return S_OK;
}

void CWood::PlaySound_Hit(_float3 vPos)
{
	m_pGameInstance->Play_Sound(TEXT("Wood_hit3"), SOUND_BLOCK_HIT, this, 1.f, vPos);

}

void CWood::Play_Create_Sound(_float3 vPos)
{
	m_pGameInstance->Play_Sound(TEXT("Wood_dig"), SOUND_BLOCK_DIG, this, 1.f, vPos);
}

HRESULT CWood::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Wood"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	__super::Ready_Components();

	return S_OK;
}

CWood* CWood::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CWood* pInstance = new CWood(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWood");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWood::Clone(void* pArg)
{
	CWood* pInstance = new CWood(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CWood");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWood::Free()
{
	//wchar_t layerName[100];
	//swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemCube"), LEVEL_YU, layerName)))
	//	return;
	//dynamic_cast<CTransform*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	//dynamic_cast<CItemCube*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))->Set_ItemTypeAndBindTexture(ITEM_WOOD);

	__super::Free();
}

