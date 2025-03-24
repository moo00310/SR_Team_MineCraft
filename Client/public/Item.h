#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CItem final : public CUIObject
{
private:
	CItem(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem(CItem& Prototype);
	virtual ~CItem() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	void Set_ItemType(ITEMNAME itemType) { m_ItemType = itemType; }

public:
	ITEMNAME	Get_ItemType() { return m_ItemType; }
	_int		Get_SlotIndex() { return m_iSlotIndexNum; }

private:
	HRESULT Ready_Components();

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	UIOBJECT_DESC Desc{};
	ITEMNAME m_ItemType = ITEMNAME_END;

private:
	_int*		m_iSlotIndex = { nullptr };
	_int		m_iSlotIndexNum;
	_bool		m_bTest = {true};

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END