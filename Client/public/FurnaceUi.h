#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "Furnace.h"

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

	void Set_FurnaceInfo(CFurnace* _furnace) { m_copyFurnace = _furnace; }

private:
	HRESULT Ready_Components();
	CFurnace* m_copyFurnace={ nullptr };

	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
public:
	static CFurnaceUi* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END