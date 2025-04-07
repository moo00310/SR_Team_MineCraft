#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
#include "ParticleEventManager.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CCollider_Cube;
class CShader;
END

BEGIN(Client)

// °Ë±â.
class CSwordAura final : public CGameObject
{
private:
	CSwordAura(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSwordAura(const CSwordAura& Prototype);
	virtual ~CSwordAura() = default;

public:
	static CSwordAura* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();

	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	CTransform* GetTransform() const;

private:
	HRESULT Ready_Components();

private:
	CTexture* m_pTextureCom = {nullptr};
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferRectCom = { nullptr };
	CCollider_Cube* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = {nullptr};
};

END