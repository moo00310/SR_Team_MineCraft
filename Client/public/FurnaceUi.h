#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "BurnUi.h"
#include "BurnResultUi.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Client)

class CFurnaceUi final : public CUIObject
{
private:
	CFurnaceUi(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFurnaceUi(CFurnaceUi& Prototype);
	virtual ~CFurnaceUi() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	void Set_FurnaceBurnUi(CBurnUi* _burnUi) { m_burnUi = _burnUi; }
	void Set_FurnaceBurnResultUi(CBurnResultUi* _burnResultUi) { m_burnResultUi = _burnResultUi; }
	void Set_Furnace(CFurnace* _furnace) { m_furnace = _furnace; }

private:
	HRESULT Ready_Components();

	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	CBurnUi* m_burnUi;
	CBurnResultUi* m_burnResultUi;
	CFurnace* m_furnace;
public:
	static CFurnaceUi* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END