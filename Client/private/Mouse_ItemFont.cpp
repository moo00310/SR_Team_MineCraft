#include "Mouse_ItemFont.h"
#include "UI_Mgr.h"

CMouse_ItemFont::CMouse_ItemFont(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUIObject{ pGraphic_Device }
{
}

CMouse_ItemFont::CMouse_ItemFont(CMouse_ItemFont& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CMouse_ItemFont::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMouse_ItemFont::Initialize(void* pArg)
{
	Desc.fSizeX = 16.f;
	Desc.fSizeY = 16.f;
	m_fOffsetX = 18.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(Desc.fX - g_iWinSizeX * 0.5f, -Desc.fY + g_iWinSizeY * 0.5f, 0.f));

	return S_OK;
}

void CMouse_ItemFont::Priority_Update(_float fTimeDelta)
{
}

void CMouse_ItemFont::Update(_float fTimeDelta)
{
	/* 아이템 폰트 좌표 업데이트 (마우스 좌표) */
	if (m_bCheck)
	{
		__super::Update(fTimeDelta);

		RECT rcRect;
		SetRect(&rcRect, (int)(Desc.fX - Desc.fSizeX * 0.5f), (int)(Desc.fY - Desc.fSizeY * 0.5f),
			(int)(Desc.fX + Desc.fSizeX * 0.5f), (int)(Desc.fY + Desc.fSizeY * 0.5f));

		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);


		Desc.fX = (float)ptMouse.x + 15.f;
		Desc.fY = (float)ptMouse.y + 12.f;

		m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3((Desc.fX - g_iWinSizeX * 0.5f) - m_fOffsetX, -Desc.fY + g_iWinSizeY * 0.5f, 0.f));
	}
}

void CMouse_ItemFont::Late_Update(_float fTimeDelta)
{
	if (m_bCheck)
	{
		__super::Late_Update(fTimeDelta);

		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
			return;
	}
}

HRESULT CMouse_ItemFont::Render()
{

	/* 10의 자리 아이템 개수 렌더 */
	if (FAILED(RendeTexture(m_pTextureCom, m_Texture_TensNum, Desc.fX, Desc.fY)))
		return E_FAIL;

	/* 1의 자리 아이템 개수 렌더 */
	if (FAILED(RendeTexture(m_pTextureCom, m_Texture_OnesNum, Desc.fX, Desc.fY)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMouse_ItemFont::RendeTexture(CTexture* pTextureCom, _int _TextureNum, _float _fX, _float _fY)
{
	if (FAILED(m_pTextureCom->Bind_Resource(_TextureNum)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	__super::Begin();
	//SetUp_RenderState();

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShaderCom->Bind_Texture("g_Texture", m_pTextureCom->Get_Texture(_TextureNum));
	m_pShaderCom->Begin(3);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(_fX - g_iWinSizeX * 0.5f, -_fY + g_iWinSizeY * 0.5f, 0.f));

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	m_pShaderCom->End();
	__super::End();
	//Reset_RenderState();

	return S_OK;
}

HRESULT CMouse_ItemFont::Ready_Components()
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMouse_ItemFont::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 80);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	return S_OK;
}

HRESULT CMouse_ItemFont::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CMouse_ItemFont* CMouse_ItemFont::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMouse_ItemFont* pInstance = new CMouse_ItemFont(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMouse_ItemFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CMouse_ItemFont::Clone(void* pArg)
{
	CMouse_ItemFont* pInstance = new CMouse_ItemFont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CMouse_ItemFont");
		Safe_Release(pInstance);
	}

	CUI_Mgr::Get_Instance()->Get_MouseItemFontlist()->push_back(pInstance);

	return pInstance;
}

void CMouse_ItemFont::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
}
