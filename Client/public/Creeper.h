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
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom[6];
	vector<BONE> vecBones;

	float Comput = {};
	int  flag = { 1 };

	float elapsedTime = 0.f;
	float maxAngle = D3DXToRadian(20.f);

private:
	HRESULT Ready_Components();
	HRESULT Ready_Bone();
	HRESULT Ready_Mesh();

	HRESULT UpDate_Mesh();


public:
	static CCreeper* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

