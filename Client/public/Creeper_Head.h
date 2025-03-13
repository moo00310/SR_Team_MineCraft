#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CCreeper_Head : public CGameObject
{
private:
	CCreeper_Head(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCreeper_Head(const CCreeper_Head& Prototype);
	virtual ~CCreeper_Head() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	void SetPos(_float3 v3) { m_pTransformCom->Set_State(Engine::CTransform::STATE_POSITION, v3); }
	CTransform* TRcom()
	{
		return m_pTransformCom;
	}

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom= { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CCreeper_Head* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

