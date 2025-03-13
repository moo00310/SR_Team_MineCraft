#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CCreeper_2: public CGameObject
{
private:
	CCreeper_2(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCreeper_2(const CCreeper_2& Prototype);
	virtual ~CCreeper_2() = default;

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
	static CCreeper_2* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

