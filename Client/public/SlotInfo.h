#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CSlotInfo final : public CUIObject 
{
private:
	CSlotInfo(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSlotInfo(CSlotInfo& Prototype);
	virtual ~CSlotInfo() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	ITEMNAME		Get_ItemName() { return m_ItemName; }
	ITEMID			Get_ItemID() { return m_ItemID; }
	_int			Get_SlotIndex() { return m_iSlotIndexNum; }
	_int			Get_ItemCount() { return m_iItemCount; }
	UIOBJECT_DESC	Get_ItemPos() { return Desc; }
	
	_bool			Get_Test() { return m_bTest; }

public:
	void Set_ItemName(ITEMNAME _ItemName) { m_ItemName = _ItemName; }
	void Set_ItemID(ITEMID _ItemID) { m_ItemID = _ItemID; }
	void Set_ItemCount(_int _itemCount) { m_iItemCount = _itemCount; }
	void Set_Test(_bool _Test) { m_bTest = _Test; }


private:
	HRESULT Ready_Components();
	HRESULT RenderItemTexture(CTexture* pTextureCom, _int _TextureNum);
	HRESULT RenderItemCount(CTexture* pTextureCom, _int _TextureNum, _float _fX, _float _fY, _float _fsizeX, _float _fsizeY);
	HRESULT SetUp_RenderState();
	HRESULT Reset_RenderState();

	void	TextureNum_Update(_int _TextureNum);

private:
	CTexture*			m_pItem_TextureCom = { nullptr };
	CTexture*			m_pItemCount_TextureCom = { nullptr };
	CTransform*			m_pItemTexture_TransformCom = { nullptr };
	CTransform*			m_pItemCountTexture_TransformCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };

private:
	UIOBJECT_DESC		Desc{};
	ITEMNAME			m_ItemName = ITEMNAME_END;
	ITEMID				m_ItemID = ITEMID_END;

private:
	_int*		m_iSlotIndex = { nullptr };
	_int		m_iSlotIndexNum		{0};
	_int		m_iCategory			{0};
	_int		m_iItemCount		{0};
	_int		m_iTensDigit		{0};
	_int		m_iOnesDigit		{0};
	_float		m_fOffsetX			{0.f};
	_bool		m_bCheck = {true};
	_bool		m_bTest = { false };


public:
	static CSlotInfo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END