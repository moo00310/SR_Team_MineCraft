#pragma once
// 모든 큐브의 부모가 되는 곳

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
END

class CCube : public CGameObject
{
protected:
	CCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCube(const CCube& Prototype);
	virtual ~CCube() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

protected:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	virtual HRESULT Ready_Components();

public:
	bool isCreeperBoom = false;

		
public:
	static CCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

