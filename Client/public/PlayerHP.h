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
	_int Get_HpCount() { return m_iHpCount; }

public:
	_int	m_iTextureNum;
	UIOBJECT_DESC Desc{};

private:
	HRESULT Ready_Components();
	void	Begin();
	void	End();



private:
	_int*	m_iHpIndex = { nullptr };
	_int	m_iHpCount;

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	static CPlayerHP* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END