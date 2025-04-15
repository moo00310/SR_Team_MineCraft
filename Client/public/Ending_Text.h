#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CEnding_Text : public CUIObject
{
public:
	typedef struct tagEnding_Text : public UIOBJECT_DESC
	{
		wstring wstrText{};
	}ENDING_TEXT_DESC;
private:
	CEnding_Text(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnding_Text(const CEnding_Text& Prototype);
	virtual ~CEnding_Text() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	wstring m_wstrText{};

private:
	HRESULT Ready_Components();

public:
	static CEnding_Text* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END