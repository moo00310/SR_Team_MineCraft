#pragma once
#include "Client_Defines.h"
#include "UIObject.h""

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)
class CItemFont final : public CUIObject
{
private:
	CItemFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItemFont(CItemFont& Prototype);
	virtual ~CItemFont() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	UIOBJECT_DESC Get_ItemPos() { return Desc; }
public:
	void Set_ItemPos(UIOBJECT_DESC _UIOBJECT_DESC) { Desc = _UIOBJECT_DESC; }

public:
	HRESULT RendeTexture(CTexture* pTextureCom, _int _TextureNun, _float _fX, _float _fY);
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();

private:
	HRESULT Ready_Components();

private:
	UIOBJECT_DESC		Desc{};

private:
	_int*	m_iIndexNum = { nullptr };
	_int	m_iIndex = { 0 };
	_int	m_iTextureNum = { 0 };
	_float  m_fOffsetX = {0.f};

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	static CItemFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
END
