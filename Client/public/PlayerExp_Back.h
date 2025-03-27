#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CPlayerExp_Back final : public CUIObject
{
private:
	CPlayerExp_Back(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerExp_Back(CPlayerExp_Back& Prototype);
	virtual ~CPlayerExp_Back() = default;

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
	void	Begin();
	void	End();

private:
	UIOBJECT_DESC Desc{};

private:
	_int* m_iExpCount = { nullptr };
	_int  m_iExpIndex = { 0 };
	_int  m_iTextureNum = { 2 };
private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

public:
	static CPlayerExp_Back* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END