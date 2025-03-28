#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CPlayerHunger_Back final : public CUIObject
{
private:
	CPlayerHunger_Back(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerHunger_Back(CPlayerHunger_Back& Prototype);
	virtual ~CPlayerHunger_Back() = default;

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
	void	Begin();
	void	End();

private:
	UIOBJECT_DESC Desc{};

private:
	_int*	m_iHungerCount = { nullptr };
	_int	m_iHungerIndex;

private:
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };

public:
	static CPlayerHunger_Back* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END