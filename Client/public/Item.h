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
	ITEMNAME	Get_ItemName() { return m_ItemName; }
	ITEMID		Get_ItemID() { return m_ItemID; }
	_int		Get_SlotIndex() { return m_iSlotIndexNum; }
	_int		Get_ItemCount() { return m_iItemCount; }


	_bool		Get_Test() { return m_bTest; }

public:
	void Set_ItemName(ITEMNAME _ItemName) { m_ItemName = _ItemName; }
	void Set_ItemCount(_int _itemCount) { m_iItemCount = _itemCount; }
	void Set_Test(_bool _Test) { m_bTest = _Test; }

private:
	HRESULT Ready_Components();

private:
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };

private:
	UIOBJECT_DESC		Desc{};
	ITEMNAME			m_ItemName = ITEMNAME_END;
	ITEMID				m_ItemID = ITEMID_END;

private:
	_int*		m_iSlotIndex = { nullptr };
	_int		m_iSlotIndexNum;
	_int		m_iItemCount = { 0 };
	_bool		m_bCheck = {true};
	_bool		m_bTest = { false };

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END