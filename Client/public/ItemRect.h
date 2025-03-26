#pragma once
// 부숴지는 큐브들의 부모가 되는 곳

#include "Client_Defines.h"
#include "Cube.h"
#include "Transform.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
END

class CItemRect : public CCube
{
public:
	typedef struct ItemPosition
	{
		_float3 position;
	}DESC;
public:
	CItemRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItemRect(const CItemRect& Prototype);
	virtual ~CItemRect() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override; 
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
public:
	void SetPos(_float3 v3) { m_pTransformCom->Set_State(CTransform::STATE_POSITION, v3); }
	_float3 GetPos() { return m_pTransformCom->Get_State(CTransform::STATE_POSITION); }
	void SetMatrix(const _float4x4& mat) { m_pTransformCom->MultiplyMatrix(mat); }
	HRESULT Set_ItemTypeAndBindTexture(ITEMTYPE _type);
protected:
	HRESULT Ready_Components();

	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	int m_iUpDownFrame;
	float m_fUpDownSpeed;

	ITEMTYPE m_eItemType;
public:
	static CItemRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

