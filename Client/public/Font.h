#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END


BEGIN(Client)
class CFont final : public CUIObject
{
private:
	CFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFont(CFont& Prototype);
	virtual ~CFont() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT Ready_Components();

private:
	ID3DXFont*	m_pFont = nullptr;
	RECT		textRect = { 0, 0, 0, 0 };
	WCHAR		m_szPath[MAX_PATH] = {};
	_wstring	m_fontPath = L"";
	_wstring	m_strText = L"";
	wchar_t		m_szBuffer[128] = {};
	_int		m_iPercent = { 0 };


public:
	static CFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};
END
