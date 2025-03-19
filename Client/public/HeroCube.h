#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider_Cube;
class CRigidbody;
END

BEGIN(Client)

class CHeroCube final : public CGameObject
{

private:
	CHeroCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHeroCube(const CHeroCube& Prototype);
	virtual ~CHeroCube() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

protected:
	_bool m_bHit{ false };

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };
	CCollider_Cube* m_pColliderCom = { nullptr };
	CRigidbody* m_pRigidbodyCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CHeroCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END