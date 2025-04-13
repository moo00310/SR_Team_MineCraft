 #include "FurnaceUi.h"

_bool g_bFurnaceUiOpen = false;

CFurnaceUi::CFurnaceUi(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CFurnaceUi::CFurnaceUi(CFurnaceUi& Prototype)
	: CUIObject( Prototype )
{
}

HRESULT CFurnaceUi::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFurnaceUi::Initialize(void* pArg)
{
	UIOBJECT_DESC Desc{};

	Desc.fSizeX = 550.f;
	Desc.fSizeY = 500.f;
	Desc.fX = g_iWinSizeX * 0.5f;
	Desc.fY = 300.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	return S_OK;
}

void CFurnaceUi::Priority_Update(_float fTimeDelta)
{
}

void CFurnaceUi::Update(_float fTimeDelta)
{
}

void CFurnaceUi::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CFurnaceUi::Render()
{
	if (g_bFurnaceUiOpen)
	{
		if (FAILED(m_pTextureCom->Bind_Resource(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pTransformCom->Bind_Resource()))
			return E_FAIL;

		__super::Begin();

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		__super::End();	

		if (m_furnace->Get_FurnaceBurn()) {
			float coalPercent = (m_furnace->GetCoalCoolTime() - m_furnace->Get_CoalTime()) / m_furnace->GetCoalCoolTime();
			float ironPercent = (m_furnace->GetIronCoolTime() - m_furnace->Get_IronTime()) / m_furnace->GetIronCoolTime();
			m_burnUi->Render(coalPercent);
			m_burnResultUi->Render(ironPercent);
		}
	}
	return S_OK;
}

HRESULT CFurnaceUi::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_FurnaceUi"), TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

CFurnaceUi* CFurnaceUi::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFurnaceUi* pInstance = new CFurnaceUi(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFurnaceUi");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CFurnaceUi::Clone(void* pArg)
{
	CFurnaceUi* pInstance = new CFurnaceUi(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CFurnaceUi");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFurnaceUi::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

}
