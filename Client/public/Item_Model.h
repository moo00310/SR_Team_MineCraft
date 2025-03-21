#pragma once
// ������ ���� �θ� �Ǵ� ��

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CItem_Model abstract : public CGameObject
{
protected:
	CItem_Model(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItem_Model(const CItem_Model& Prototype);
	virtual ~CItem_Model() = default;

public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg) = 0;
	virtual void Priority_Update(_float fTimeDelta) = 0;
	virtual void Update(_float fTimeDelta) = 0;
	virtual void Late_Update(_float fTimeDelta) = 0;
	virtual HRESULT Render() = 0;

protected:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };

protected:
	wstring m_TextureTag = {};
	LEVEL m_TextureLevel = {};

	vector<KEYFREAME> m_swing = {};
	float fElapsedTime = 0.0f;

	virtual HRESULT Ready_Components();

private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

public:
	static CItem_Model* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};


