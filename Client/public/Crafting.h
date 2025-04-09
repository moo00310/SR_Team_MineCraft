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
		map<int, ITEMNAME> inputPattern;
		ITEMNAME resultItem = ITEMNAME_END;
		int resultCount = { 0 };
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

private:
	HRESULT Ready_Components();

private:
	_int		m_iItemCount = { 0 };
	_int		m_iSlotIndex = { 0 };
	ITEMNAME	m_ItemName = ITEMNAME_END;

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
