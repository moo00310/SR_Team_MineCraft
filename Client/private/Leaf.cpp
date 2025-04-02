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

HRESULT CLeaf::Delete_Cube(_float3 fPos)
{
	for (size_t i = 0; i < m_vecPositions.size(); ++i)
	{
		if (m_vecPositions[i].x == fPos.x &&
			m_vecPositions[i].y == fPos.y &&
			m_vecPositions[i].z == fPos.z)
		{
			if (FAILED(Delete_Component(TEXT("Com_Collider_Cube"), m_Colliders[i])))
				return E_FAIL;

			int random = rand() % 100;
			if (random < 10) {
				wchar_t layerName[100];
				swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);
				if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
					return E_FAIL;
				if (CItemRect* _copy = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
					_copy->SetPos(m_vecPositions[i]);
					_copy->Set_ItemTypeAndBindTexture(ITEMNAME_SAPLING);
					_copy->Set_Bright(m_vecBrights[i]);
				}
			}
			else if (10 <= random && random < 20) {
				wchar_t layerName[100];
				swprintf(layerName, 100, L"Layer_Chunk%d", m_iMyChunk);
				if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
					return E_FAIL;
				if (CItemRect* _copy = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
					_copy->SetPos(m_vecPositions[i]);
					_copy->Set_ItemTypeAndBindTexture(ITEMNAME_APPLE);
					_copy->Set_Bright(m_vecBrights[i]);
				}
			}

			// 2. 벡터에서 해당 위치 제거
			m_vecPositions.erase(m_vecPositions.begin() + i);
			m_vecBrights.erase(m_vecBrights.begin() + i);

			// 3. 콜라이더 제거
			Safe_Release(m_Colliders[i]);
			m_Colliders.erase(m_Colliders.begin() + i);

			// 4. 인스턴스 버퍼 업데이트
			m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions, m_vecBrights);


			return S_OK;
		}
	}

	return E_FAIL;
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

