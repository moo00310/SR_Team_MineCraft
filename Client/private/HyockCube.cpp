#include "HyockCube.h"

CHyockCube::CHyockCube(LPDIRECT3DDEVICE9 pGraphic_Device) : CCube(pGraphic_Device)
{
}

CHyockCube::CHyockCube(const CHyockCube& Prototype) : CCube(Prototype)
{
}

CHyockCube* CHyockCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHyockCube* pInstance = new CHyockCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHyockCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CHyockCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHyockCube::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CHyockCube::Priority_Update(_float fTimeDelta)
{
}

void CHyockCube::Update(_float fTimeDelta)
{
	int a = 10;
}

void CHyockCube::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CHyockCube::Render()
{
	// 다이아 블럭 렌더링.
	if (FAILED(m_TextureDiamondOreCom->Bind_Resource(0)))
	{
		return E_FAIL;
	}

	if (FAILED(m_CubeCom->Bind_Buffers()))
	{
		return E_FAIL;
	}

	if (FAILED(m_CubeCom->Render()))
	{
		return E_FAIL;
	}

	// 금 간 텍스쳐는 투명도 적용 후 렌더링.
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 150);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (FAILED(m_TextureDestroyCom->Bind_Resource(8)))
	{
		return E_FAIL;
	}

	if (FAILED(m_CubeOnlyCom->Bind_Buffers()))
	{
		return E_FAIL;
	}
	
	if (FAILED(m_CubeOnlyCom->Render()))
	{
		return E_FAIL;
	}	

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	

	return S_OK;
}

CGameObject* CHyockCube::Clone(void* pArg)
{
	CHyockCube* pInstance = new CHyockCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone : CHyockCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHyockCube::Free()
{
	__super::Free();

	Safe_Release(m_TextureDestroyCom);
	Safe_Release(m_TextureDiamondOreCom);
	Safe_Release(m_CubeOnlyCom);
	Safe_Release(m_CubeCom);
}

HRESULT CHyockCube::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Destroy"),
		TEXT("Com_Texture_Destroy"), reinterpret_cast<CComponent**>(&m_TextureDestroyCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Cube_Destroy"),
		TEXT("Com_Component_Destroy"), reinterpret_cast<CComponent**>(&m_CubeOnlyCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Texture_Diamond_ore"),
		TEXT("Com_Texture_Diamond_Ore"), reinterpret_cast<CComponent**>(&m_TextureDiamondOreCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_HYEOK, TEXT("Prototype_Component_Cube_Diamond_Ore"),
		TEXT("Com_Component_Diamond_Ore"), reinterpret_cast<CComponent**>(&m_CubeCom))))
		return E_FAIL;

	return S_OK;
}
