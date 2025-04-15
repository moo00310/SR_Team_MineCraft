#include "Ending_Background.h"

CEnding_Background::CEnding_Background(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUIObject(pGraphic_Device)
{
}

CEnding_Background::CEnding_Background(const CEnding_Background& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CEnding_Background::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnding_Background::Initialize(void* pArg)
{
	Desc.fSizeX = 2.f;
	Desc.fSizeY = 2.f;
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

void CEnding_Background::Priority_Update(_float fTimeDelta)
{
}

void CEnding_Background::Update(_float fTimeDelta)
{
	m_fTime += fTimeDelta;
}


void CEnding_Background::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CEnding_Background::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTime * 30) % 111))) //111장 0~110
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnding_Background::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_ENDING, TEXT("Prototype_Component_Texture_Ending_Background"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CEnding_Background* CEnding_Background::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnding_Background* pInstance = new CEnding_Background(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnding_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnding_Background::Clone(void* pArg)
{
	CEnding_Background* pInstance = new CEnding_Background(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CEnding_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnding_Background::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
