#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider_Cube;
END

BEGIN(Client)

class CHeroCubeNoMove final : public CGameObject
{

private:
	CHeroCubeNoMove(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHeroCubeNoMove(const CHeroCubeNoMove& Prototype);
	virtual ~CHeroCubeNoMove() = default;

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

private:
	HRESULT Ready_Components();

public:
	static CHeroCubeNoMove* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END