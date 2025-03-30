#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CPlayerHP_Back final : public CUIObject
{
private:
	CPlayerHP_Back(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerHP_Back(CPlayerHP_Back& Prototype);
	virtual ~CPlayerHP_Back() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	HRESULT Ready_Components();
	HRESULT	SetUp_RenderState();
	HRESULT	Reset_RenderState();

private:
	UIOBJECT_DESC Desc{};

private:
	_int*	m_iHpCount = { nullptr };
	_int	m_iHpIndex;

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	static CPlayerHP_Back* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END