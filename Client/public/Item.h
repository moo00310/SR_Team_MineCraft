#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)
class CItem final : public CUIObject
{
private:
	CItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem(CItem& Prototype);
	virtual ~CItem() = default;


public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;


public: 
	void Clear_ItemTextRender();

public:
	void	ItemMatch(ITEMNAME _ItemName, _bool* _bTestInfo, _float _fX, _float _fY);

	_wstring	Get_ItemInfoText() { return m_strText; }

private:
	HRESULT Ready_Components();

private:
	WCHAR		m_szPath[MAX_PATH] = {};
	_wstring	m_fontPath = L"";
	_wstring	m_strText = L"";
	_bool		m_bTestInfo = { false };
	_float		m_fX = { 0.f };
	_float		m_fY = { 0.f };
	_bool*		m_bItemTextRender = { nullptr};


private:
	ID3DXFont*	m_pFont = nullptr;
	RECT		textRect = {0, 0, 0, 0};
	map<ITEMNAME, _wstring> m_ItemMap;

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

};
END