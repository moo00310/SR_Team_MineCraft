#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CCheckBox final : public CUIObject
{
private:
	CCheckBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCheckBox(CCheckBox& Prototype);
	virtual ~CCheckBox() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	_int Get_CheckBoxIndex() { return m_iCheckBoxIndex; }

public:
	void Set_CheckBoxIndex(_int _index) { m_iCheckBoxIndex = _index; }

public:
	UIOBJECT_DESC Desc{};

private:
	HRESULT Ready_Components();

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float m_fCheckBox = { 0.f };
	_int   m_iCheckBoxIndex = { 0 };

public:
	static CCheckBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END