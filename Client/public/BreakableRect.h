#pragma once
// 부숴지는 큐브들의 부모가 되는 곳

#include "Client_Defines.h"
#include "Cube.h"
#include "Transform.h"

BEGIN(Engine)
class CVIBuffer_Rect3D;
class CCollider_Cube;
END

class CBreakableRect : public CCube
{
public:
	CBreakableRect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBreakableRect(const CBreakableRect& Prototype);
	virtual ~CBreakableRect() = default;

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
	void Set_RenderActive(bool _b) { m_bRenderActive = _b; }
	bool Get_RenderActive() { return m_bRenderActive; }
protected:
	HRESULT Ready_Components();
	bool m_bRenderActive = true;

	CVIBuffer_Rect3D* m_pVIBufferCom = { nullptr};
	CCollider_Cube* m_pColliderCom = { nullptr };
	
public:
	static CBreakableRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

