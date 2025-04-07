#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CInventory_Back final : public CUIObject
{
private:
	CInventory_Back(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInventory_Back(CInventory_Back& Prototype);
	virtual ~CInventory_Back() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;


	//ITEMID	Get_BackID() {return m_eBackID;}

private:
	HRESULT Ready_Components();

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	UIOBJECT_DESC Desc{};

private:
	_int*	m_iSlotIndex = { nullptr };
	_int	m_iSlotIndexNum;
	_int	m_iCategory;
	_int	m_iTextureNum;
	//ITEMID	m_eBackID;

public:
	static CInventory_Back* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END