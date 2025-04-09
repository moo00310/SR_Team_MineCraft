#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CFurnaceUi final : public CUIObject
{
private:
	CFurnaceUi(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFurnaceUi(CFurnaceUi& Prototype);
	virtual ~CFurnaceUi() = default;

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
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:;
	_bool m_bEKeyPressed = { false };

public:
	static CFurnaceUi* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END