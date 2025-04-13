#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Client)

// ¾ÆÀÌÅÛ È¹µæÇÏ¸é UI¿¡ ¶ç¿ì°í °¡¹æÀ¸·Î 
// µé¾î°¥ UI ¿ÀºêÁ§Æ®.
class CUIDropItem : public CUIObject
{
private:
	CUIDropItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CUIDropItem(CUIDropItem& Prototype);
	virtual ~CUIDropItem() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	static CUIDropItem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

	virtual void OnPushPool() override;
	void SetTransform(_float2 _position);
	void SetDirection(_float2 _direction);
	CTransform* GetTransform() const;
	void SetTargetPosition(_float2 _target);
	void SetItemName(ITEMNAME _itemName);

private:
	// ÇöÀç ÁÂÇ¥°¡ UI ÁÂÇ¥¸¦ ¹þ¾î³µ´Â°¡? XÁÂÇ¥.
	_bool IsOutScreenX(_float& _outX);

	// ÇöÀç ÁÂÇ¥°¡ UI ÁÂÇ¥¸¦ ¹þ¾î³µ´Â°¡? YÁÂÇ¥.
	_bool IsOutScreenY(_float& _outY);

private:
	HRESULT Ready_Components();

	// UI ÁÂÇ¥°è ±âÁØ ¹æÇâ º¤ÅÍ.
	_float2 m_vDirection = {0.f, 0.f};

	// µµÂø º¤ÅÍ.
	_float2 m_vTargetPosition;

	ITEMNAME m_kITEM_NAME;

private:
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = {nullptr};
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
};

END