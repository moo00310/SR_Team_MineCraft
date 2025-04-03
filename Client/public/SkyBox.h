#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_CubeShader;
class CShader;
END

BEGIN(Client)

class CSkyBox final : public CGameObject
{
private:
	CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSkyBox(const CSkyBox& Prototype);
	virtual ~CSkyBox() = default;

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
	CVIBuffer_CubeShader* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

private:
	HRESULT Ready_Components();
public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END