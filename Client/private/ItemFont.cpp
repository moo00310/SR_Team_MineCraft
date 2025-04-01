#include "ItemFont.h"
#include "UI_Mgr.h"

CItemFont::CItemFont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CItemFont::CItemFont(CItemFont& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CItemFont::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItemFont::Initialize(void* pArg)
{
	m_iIndexNum = (int*)pArg;
	m_iIndex = *m_iIndexNum;

	Desc.fSizeX = 14.f;
	Desc.fSizeY = 14.f;
	Desc.fX = 482.f;
	Desc.fY = 520.f;
	m_fOffsetX = 18.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));


	return S_OK;
}

void CItemFont::Priority_Update(_float fTimeDelta)
{
}

void CItemFont::Update(_float fTimeDelta)
{
}

void CItemFont::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CItemFont::Render()
{
	/* 인벤토리 열려있는지 확인 */
	if (!g_bMainInventoryOpen)
		return S_OK;
	
	/* 첫 번째 텍스쳐 렌더링 */
	if (FAILED(RendeTexture(m_pTextureCom, 8, Desc.fX, Desc.fY)))
		return E_FAIL;

	/* 두 번째 텍스쳐 렌더링 */
	if (FAILED(RendeTexture(m_pTextureCom, 8, Desc.fX - m_fOffsetX, Desc.fY)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItemFont::RendeTexture(CTexture* pTextureCom, _int _TextureNun, _float _fX, _float _fY)
{
	if (FAILED(m_pTextureCom->Bind_Resource(_TextureNun)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_fX - g_iWinSizeX * 0.5f, -_fY + g_iWinSizeY * 0.5f, 0.f));

	__super::Begin();
	SetUp_RenderState();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	__super::End();
	Reset_RenderState();

	return S_OK;
}

HRESULT CItemFont::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 80);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CItemFont::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CItemFont::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Font"), TEXT("Com_Texture"),
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

CItemFont* CItemFont::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CItemFont* pInstance = new CItemFont(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CItemFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CItemFont::Clone(void* pArg)
{
	CItemFont* pInstance = new CItemFont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CItemFont");
		Safe_Release(pInstance);
	}

	CUI_Mgr::Get_Instance()->Get_vecItemFontlist()->push_back(pInstance);

	return pInstance;
}

void CItemFont::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}
