#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CPlayerHunger final : public CUIObject
{
private:
	CPlayerHunger(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerHunger(CPlayerHunger& Prototype);
	virtual ~CPlayerHunger() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	_int	Get_TextureNum()	{ return m_iTextureNum; }
	_bool	Get_Flicker()const	{ return m_bFlicker; }

public:
	void Set_TextureNum(_int TextureNum)	{ m_iTextureNum = TextureNum; }
	void Set_Flicker(_bool Flicker)			{ m_bFlicker = Flicker; }

private:
	HRESULT Ready_Components();
	void	Begin();
	void	End();

private:
	_int*	m_iHungerCount = { nullptr };
	_int	m_iHungerIndex = {0};
	_int	m_iTextureNum = {0};
	_bool	m_bFlicker;
	_float	m_fTiem;

private:
	UIOBJECT_DESC Desc{};

private:
	CTexture*		m_pTextureCom = { nullptr };
	CTransform*		m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };

public:
	static CPlayerHunger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END