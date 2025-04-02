#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)


class CMouse_ItemFont final : public CUIObject
{
private:
	CMouse_ItemFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMouse_ItemFont(CMouse_ItemFont& Prototype);
	virtual ~CMouse_ItemFont() = default;

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
	void Set_Check(_bool _bCheck) { m_bCheck = _bCheck; }
	void Set_ItemName(ITEMNAME _ItemName) { m_ItemName = _ItemName; }
	void Set_ItemPos(UIOBJECT_DESC _Desc) { Desc = _Desc; }
	void Set_ItemFont_Tens(_int _TextureTens) { m_Texture_TensNum = _TextureTens; }
	void Set_ItemFont_Ones(_int _TextureOnes) { m_Texture_OnesNum = _TextureOnes; }

	

private:
	HRESULT RendeTexture(CTexture* pTextureCom, _int _TextureNun, _float _fX, _float _fY);
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
	_int		m_Texture_TensNum = { 0 };
	_int		m_Texture_OnesNum = { 0 };
	_float		m_fOffsetX = { 0.f };
	ITEMNAME	m_ItemName = ITEMNAME_END;

public:
	static CMouse_ItemFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free();
};

END