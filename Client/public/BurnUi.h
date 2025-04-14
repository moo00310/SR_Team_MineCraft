#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "Furnace.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CTransform;
class CShader;
END

BEGIN(Client)

class CBurnUi final : public CUIObject
{
private:
	CBurnUi(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBurnUi(CBurnUi& Prototype);
	virtual ~CBurnUi() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render(_float _coalTime);

private:
	HRESULT Ready_Components();

	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
public:
	static CBurnUi* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END