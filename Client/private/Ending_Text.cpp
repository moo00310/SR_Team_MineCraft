#include "Ending_Text.h"

CEnding_Text::CEnding_Text(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CUIObject(pGraphic_Device)
{
}

CEnding_Text::CEnding_Text(const CEnding_Text& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CEnding_Text::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnding_Text::Initialize(void* pArg)
{
	ENDING_TEXT_DESC* tDesc = static_cast<ENDING_TEXT_DESC*>(pArg);

	m_fX = tDesc->fX;
	m_fY = tDesc->fY;
	m_wstrText = tDesc->wstrText;

	return S_OK;
}

void CEnding_Text::Priority_Update(_float fTimeDelta)
{
}

void CEnding_Text::Update(_float fTimeDelta)
{
	if (0.1f < fTimeDelta)
		return;

	m_fY -= fTimeDelta * 30.f;
}

void CEnding_Text::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CEnding_Text::Render()
{
	__super::Begin();

	// 특정 위치
	_int iX = static_cast<_int>(m_fX); // 원하는 X 위치
	_int iY = static_cast<_int>(m_fY); // 원하는 Y 위치

	// 폰트 높이 추정 (예: 32px 폰트라면 높이 48 정도)
	_int iTextHeight = 48;

	// 화면 너비
	_int iScreenWidth = g_iWinSizeX;

	// 중앙 정렬을 위한 RECT 생성
	RECT rect = {
		iX - (iScreenWidth >> 1),   // 왼쪽
		iY - (iTextHeight >> 1),    // 위쪽
		iX + (iScreenWidth >> 1),   // 오른쪽
		iY + (iTextHeight >> 1)     // 아래쪽
	};

	// 텍스트 출력
	g_pTitleFont->DrawTextW(
		nullptr,
		m_wstrText.c_str(),
		-1,
		&rect,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	__super::End();

	return S_OK;
}

HRESULT CEnding_Text::Ready_Components()
{
	/* For.Com_Transform */
	//CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
	//	TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
	//	return E_FAIL;

	return S_OK;
}

CEnding_Text* CEnding_Text::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEnding_Text* pInstance = new CEnding_Text(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEnding_Text");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnding_Text::Clone(void* pArg)
{
	CEnding_Text* pInstance = new CEnding_Text(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CEnding_Text");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnding_Text::Free()
{
	__super::Free();
}
