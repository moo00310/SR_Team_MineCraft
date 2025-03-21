#include "UIObject.h"

CUIObject::CUIObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CUIObject::CUIObject(const CUIObject& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CUIObject::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
	D3DXMatrixIdentity(&m_ViewMatrix);

	D3DVIEWPORT9            ViewportDesc{};
	m_pGraphic_Device->GetViewport(&ViewportDesc);

	/* w, h, n, f : �亼���� �����Ѵ� */
	D3DXMatrixOrthoLH(&m_ProjMatrix, static_cast<_float>(ViewportDesc.Width), static_cast<_float>(ViewportDesc.Height), 0.f, 1.f);

	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fX = pDesc->fX;
	m_fY = pDesc->fY;
	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;

	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
}

void CUIObject::Update(_float fTimeDelta)
{
}

void CUIObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIObject::Render()
{
	return S_OK;
}
void CUIObject::Begin()
{
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void CUIObject::End()
{
	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_OldViewMatrix);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_OldProjMatrix);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

_bool CUIObject::isPick(HWND hWnd)
{
	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);
	
	RECT			rcUI = 
	{ 
		static_cast<_long>(m_fX - m_fSizeX * 0.5f),
		static_cast<_long>(m_fY - m_fSizeY * 0.5f),
		static_cast<_long>(m_fX + m_fSizeX * 0.5f),
		static_cast<_long>(m_fY + m_fSizeY * 0.5f)
	};

	return PtInRect(&rcUI, ptMouse);
}



void CUIObject::Free()
{
	__super::Free();
}
