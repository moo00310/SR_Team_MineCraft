#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)


class CMouse_Item final : public CUIObject
{
private:
	CMouse_Item(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMouse_Item(CMouse_Item& Prototype);
	virtual ~CMouse_Item() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	_bool		Get_Check() { return m_bCheck; }

public:
	void Set_Check(_bool _bCheck)			{ m_bCheck = _bCheck; }
	void Set_ItemName(ITEMNAME _ItemName)	{ m_ItemName = _ItemName; }

private:
	HRESULT Ready_Components();
	HRESULT	SetUp_RenderState();
	HRESULT	Reset_RenderState();

private:
	UIOBJECT_DESC Desc{};

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_bool		m_bCheck = { false };

	ITEMNAME	m_ItemName = ITEMNAME_END;

public:
	static CMouse_Item* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free();
};

END