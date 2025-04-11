#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)
class CCrafting final : public CUIObject
{
private:
	CCrafting(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCrafting(CCrafting& Prototype);
	virtual ~CCrafting() = default;

public:
	typedef struct tagRecipeDesc
	{
		map<_int, ITEMNAME> inputPattern;
		ITEMNAME resultItem = ITEMNAME_END;
		_int resultCount = { 0 };
		_int requiredItemCounts = { 0 };

	}RECIPE_DESC;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	void Crafing();
	void RButton();

private:
	HRESULT Ready_Components();

private:
	_int		m_iItemTotalCount = { 0 };
	_int		m_iSlotTotalCount = { 0 };
	_int		m_iresultCount = { 0 };
	_int		m_iTempCount = { 0 };
	_int		m_iSlotIndex = { 0 };
	ITEMNAME	m_ItemName = ITEMNAME_END;
	
	_int		m_iRbuttonTotal = { 0 };



	_bool		m_bMatch = { true };


	vector<RECIPE_DESC> m_vecRecipelist;

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	static CCrafting* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};
END
