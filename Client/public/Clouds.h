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
class CClouds : public CGameObject
{
private:
	CClouds(LPDIRECT3DDEVICE9 pGraphic_Device);
	CClouds(const CClouds& Prototype);
	virtual ~CClouds() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	//블럭 밝기 조절
	void Set_Bright(float _f) { m_fBright = _f; }
private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
private:
	HRESULT Ready_Components();

	float m_fBright = 1;
public:
	static CClouds* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END