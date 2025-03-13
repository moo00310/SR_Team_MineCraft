#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CCreeper : public CGameObject
{
private:
	CCreeper(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCreeper(const CCreeper& Prototype);
	virtual ~CCreeper() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTransform* m_pTransformCom = { nullptr };

private:
	//vector<BONE*> Bones;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Bone();

public:
	static CCreeper* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

