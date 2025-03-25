#include "Wood.h"

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

	return S_OK;
}

void CWood::Priority_Update(_float fTimeDelta)
{
}

void CWood::Update(_float fTimeDelta)
{
}

void CWood::Late_Update(_float fTimeDelta)
{
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

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::Render();

	return S_OK;
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
	wchar_t layerName[100];
	swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemWood"), LEVEL_YU, layerName)))
		return;
	dynamic_cast<CTransform*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName)->Find_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	__super::Free();
}

