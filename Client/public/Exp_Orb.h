#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CShader;
END

BEGIN(Client)
class CExp_Orb : public CGameObject
{
private:
	CExp_Orb(LPDIRECT3DDEVICE9 pGraphic_Device);
	CExp_Orb(const CExp_Orb& Prototype);
	virtual ~CExp_Orb() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;


public:
	CTransform* Get_Transform() const { return m_pTransformCom; }

private:
	void Look_At_Camera();

private:
	//_uint m_iTexture{ 13 };
	_float m_fTime{ 0.f };




private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CRigidbody* m_pRigidbodyCom{ nullptr };
	
private:
	HRESULT Ready_Components();
public:
	static CExp_Orb* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END