#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CArm_Steve : public CGameObject
{
public:
	typedef struct tagArmesc 
	{
		CGameObject* pTarget = { nullptr };
	} DESC;

private:
	CArm_Steve(LPDIRECT3DDEVICE9 pGraphic_Device);
	CArm_Steve(const CArm_Steve& Prototype);
	virtual ~CArm_Steve() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };

	float Comput = {};
	int  flag = { 1 };
	int m_bisTPS = { -1 };

private:
	HRESULT Ready_Components();

public:
	static CArm_Steve* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};