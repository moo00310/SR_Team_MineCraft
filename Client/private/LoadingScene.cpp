#include "LoadingScene.h"
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include "Loader.h"

CLoadingScene::CLoadingScene(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CLoadingScene::CLoadingScene(CLoadingScene& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CLoadingScene::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingScene::Initialize(void* pArg)
{
	GetModuleFileName(nullptr, m_szPath, MAX_PATH);
	PathRemoveFileSpec(m_szPath);

	m_fontPath = _wstring(m_szPath) + L"\\Resources\\Fonts\\font.ttf";
	if (!AddFontResourceEx(m_fontPath.c_str(), FR_PRIVATE, 0))
	{
		MSG_BOX("Failed to Created : ItemFont");
		return E_FAIL;
	}

	if (m_pFont == nullptr)
	{
		D3DXFONT_DESC fontDesc = {};
		fontDesc.Height = 34;
		fontDesc.Width = 0;
		fontDesc.Weight = FW_NORMAL;
		fontDesc.CharSet = DEFAULT_CHARSET;
		fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
		fontDesc.Quality = CLEARTYPE_QUALITY;
		fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		lstrcpy(fontDesc.FaceName, L"Galmuri7 Regular");


		if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &fontDesc, &m_pFont)))
		{
			MSG_BOX("폰트 생성 실패");
		}
	}

	SetRect(&textRect, 0, 0, g_iWinSizeX, g_iWinSizeY);

	UIOBJECT_DESC Desc{};

	Desc.fSizeX = g_iWinSizeX;
	Desc.fSizeY = g_iWinSizeY;
	Desc.fX = g_iWinSizeX * 0.5f;
	Desc.fY = g_iWinSizeY * 0.5f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	return S_OK;
}

void CLoadingScene::Priority_Update(_float fTimeDelta)
{
	if (m_pLoader != nullptr)
	{
		m_iPercent = m_pLoader->Get_Percent();

		swprintf(m_szBuffer, 128, L"Loading... %d%%", m_iPercent);
		m_strText = m_szBuffer;
	}
}

void CLoadingScene::Update(_float fTimeDelta)
{
	

}

void CLoadingScene::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CLoadingScene::Render()
{
	if (g_bChangeLevel && m_pLoader != nullptr)
	{
		if (FAILED(m_pTextureCom->Bind_Resource(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pTransformCom->Bind_Resource()))
			return E_FAIL;

		__super::Begin();
		//if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
		//	return E_FAIL;
		//if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		//	return E_FAIL;
		//if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		//	return E_FAIL;


		//m_pShaderCom->Bind_Texture("g_Texture", m_pTextureCom->Get_Texture(0));
		//m_pShaderCom->SetFloat("g_LoadingPercent", m_fTime);
		//m_pShaderCom->Begin(7);

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;


		//m_pShaderCom->End();
		__super::End();

		m_pFont->DrawTextW(
			NULL,
			m_strText.c_str(),
			-1,
			&textRect,
			DT_NOCLIP,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);

	}

	return S_OK;
}

HRESULT CLoadingScene::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading"), TEXT("Com_Texture"),
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

CLoadingScene* CLoadingScene::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLoadingScene* pInstance = new CLoadingScene(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLoadingScene");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CLoadingScene::Clone(void* pArg)
{
	CLoadingScene* pInstance = new CLoadingScene(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CLoadingScene");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoadingScene::Free()
{
	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}

	RemoveFontResourceEx(m_fontPath.c_str(), FR_PRIVATE, 0);
	
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
}
