#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CBag final : public CUIObject
{
private:
	CBag(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBag(CBag& Prototype);
	virtual ~CBag() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	void TestBag();


private:
	HRESULT Ready_Components();
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();

private:
	UIOBJECT_DESC Desc{};

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	static CBag* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END