#include "Crosshair.h"

CCrosshair::CCrosshair(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUIObject{ pGraphic_Device }
{

}

CCrosshair::CCrosshair(CCrosshair& Prototype)
	:CUIObject(Prototype)
{

}

HRESULT CCrosshair::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCrosshair::Initialize(void* pArg)
{
	Desc.fSizeX = 25.f;
	Desc.fSizeY = 25.f;
	Desc.fX = g_iWinSizeX >> 1;
	Desc.fY = g_iWinSizeY >> 1;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	return S_OK;
}

void CCrosshair::Priority_Update(_float fTimeDelta)
{
}

void CCrosshair::Update(_float fTimeDelta)
{
}

void CCrosshair::Late_Update(_float fTimeDelta)
{
	if (!isOn)
		return;

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CCrosshair::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    __super::Begin();

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    __super::End();


    return S_OK;
}

void CCrosshair::On()
{
	isOn = true;
}

void CCrosshair::Off()
{
	isOn = false;
}

HRESULT CCrosshair::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Crosshair"), TEXT("Com_Texture"),
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

CCrosshair* CCrosshair::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCrosshair* pInstance = new CCrosshair(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCrosshair");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCrosshair::Clone(void* pArg)
{
	CCrosshair* pInstance = new CCrosshair(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCrosshair");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCrosshair::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}
