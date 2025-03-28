#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CPlayerLevel final : public CUIObject
{
private:
	CPlayerLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerLevel(CPlayerLevel& Prototype);
	virtual ~CPlayerLevel() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	_int	Get_TextureNum() { return m_iTextureNum; }
	_int	Get_LevelIndex() { return m_iLevelIndex; }
	_bool	Get_RenderOn() { return m_bRenderOn; }

public:
	void Set_TextureNum(_int TextureNum) { m_iTextureNum = TextureNum; }
	void Set_RenderOn(_bool _bRenderOn) { m_bRenderOn = _bRenderOn; }

private:
	HRESULT Ready_Components();
	void	Begin();
	void	End();

private:
	_int*	m_iLevelCount = { nullptr };
	_int	m_iLevelIndex = { 0 };
	_int	m_iTextureNum = { 0 };
	_bool	m_bRenderOn = { false };

private:
	UIOBJECT_DESC Desc{};

private:
	CTexture*		m_pTextureCom = { nullptr };
	CTransform*		m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader*		m_pShaderCom = { nullptr };

public:
	static CPlayerLevel* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END