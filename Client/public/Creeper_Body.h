#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CCreeper_Body : public CGameObject
{
private:
	CCreeper_Body(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCreeper_Body(const CCreeper_Body& Prototype);
	virtual ~CCreeper_Body() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };

public:
	void SetPos(_float3 v3) { m_pTransformCom->Set_State(CTransform::STATE_POSITION, v3); }

private:
	HRESULT Ready_Components();

public:
	static CCreeper_Body* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

