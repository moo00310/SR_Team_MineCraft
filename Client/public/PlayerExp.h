#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CPlayerExp final : public CUIObject
{
private:
	CPlayerExp(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerExp(CPlayerExp& Prototype);
	virtual ~CPlayerExp() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	_int	Get_TextureNum()	{ return m_iTextureNum; }
	_int	Get_ExpIndex()		{ return m_iExpIndex; }
	_bool	Get_RenderOn()		{ return m_bRenderOn; }
	
public:
	void Set_TextureNum(_int TextureNum) { m_iTextureNum = TextureNum; }
	void Set_RenderOn(_bool _bRenderOn) { m_bRenderOn = _bRenderOn; }

private:
	HRESULT Ready_Components();
	HRESULT	SetUp_RenderState();
	HRESULT	Reset_RenderState();

private:
	_int*	m_iExpCount = { nullptr };
	_int	m_iExpIndex = { 0 };
	_int	m_iTextureNum = { 0 };
	_bool	m_bRenderOn = { false };

private:
	UIOBJECT_DESC Desc{};

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	static CPlayerExp* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END