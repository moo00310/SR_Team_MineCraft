#pragma once
// 아이템 모델의 부모가 되는 곳

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

class CItem_Model abstract : public CGameObject
{
public:
	typedef struct tagItemDesc
	{
		wstring TextureTag = {};
		LEVEL TextureLevel = {};
	}DESC;

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

protected:
	BONE m_Bone = {};
	map<_uint, vector<KEYFREAME>> m_Animations = {};
	float fElapsedTime = 0.0f;

protected:
	virtual HRESULT Ready_Components();
	HRESULT Update_Anime(_int _type, _float fTimeDelta);
	HRESULT Update_BoneAndMesh(const Matrix& matrix);


private:
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	virtual HRESULT Ready_Bone() = 0;
	virtual HRESULT Ready_Anime() = 0;

public:
	virtual void Free();
};


