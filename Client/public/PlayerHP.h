#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CPlayerHP final : public CUIObject
{
private:
	CPlayerHP(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerHP(CPlayerHP& Prototype);
	virtual ~CPlayerHP() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	_int	Get_TextureNum()	{ return m_iTextureNum;}
	_bool	Get_Flicker()const	{ return m_bFlicker; }

public:
	void Set_TextureNum(_int TextureNum)	{ m_iTextureNum = TextureNum; }
	void Set_Flicker(_bool Flicker) { m_bFlicker = Flicker; if (Flicker)  m_fFlickerTime = 0.f; }

private:
	HRESULT Ready_Components();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();

private:
	_int*	m_iHpCount = { nullptr };
	_int	m_iHpIndex;
	_int	m_iTextureNum;
	_float	m_fTime = { 0.f };
	_bool	m_bFlicker;
	_float  m_fFlickerTime = { 0.f };


private:
	UIOBJECT_DESC Desc{};

private:
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };


public:
	static CPlayerHP* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END