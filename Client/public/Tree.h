#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END
class CBreakableCube;

class CTree : public CGameObject
{
public:
	typedef struct TreeOption
	{
		//_float4x4 vPos;
		int iHeight;

	}DESC;

private:
	CTree(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTree(const CTree& Prototype);
	virtual ~CTree() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

private:
	CTransform* m_pTransformCom = { nullptr };
	int index = {};


	int m_iHeight = {};
	int m_iAddLeaf = {};

	_float4x4 m_Matrix = {};

	_float2 leaf[20] =
	{
		{0.f,	1.f},
		{0.f,	-1.f},
		{1.f,	0.f},
		{-1.f,	 0.f},

		{-1.f,	1.f},
		{1.f,	1.f},
		{1.f,	-1.f},
		{-1.f,	-1.f},

		{2.f, 1.f},
		{2.f, 0.f},
		{2.f, -1.f},

		{1.f, -2.f},
		{0.f, -2.f},
		{-1.f, -2.f},
	
		{-2.f, 1.f},
		{-2.f, 0.f},
		{-2.f, -1.f},

		{-1.f, 2.f},
		{0.f, 2.f},
		{1.f, 2.f},
	};


private:
	HRESULT Ready_Components();
	HRESULT Ready_Objects(int height);
	HRESULT Ready_Pos(int height);

public:
	static CTree* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

