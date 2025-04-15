#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CEnding_Background : public CUIObject
{
private:
	CEnding_Background(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnding_Background(const CEnding_Background& Prototype);
	virtual ~CEnding_Background() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	_float m_fTime{ 0.f };
private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	UIOBJECT_DESC	Desc{};

private:
	HRESULT Ready_Components();
public:
	static CEnding_Background* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END