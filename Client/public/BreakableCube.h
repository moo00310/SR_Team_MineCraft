#pragma once
// 부숴지는 큐브들의 부모가 되는 곳

#include "Client_Defines.h"
#include "Cube.h"
#include "Transform.h"
#include "ItemCube.h"
#include "ItemRect.h"

BEGIN(Engine)
class CVIBuffer_CubeInstance;
class CCollider_Cube;
class CShader;
END

class CBreakableCube : public CCube
{
public:
	CBreakableCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBreakableCube(const CBreakableCube& Prototype);
	virtual ~CBreakableCube() = default;

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
	void Set_InstanceBuffer(vector<D3DXVECTOR3> _objects) { m_pVIBufferCom->Update_InstanceBuffer(_objects); }
	bool Get_RenderActive() { return m_bRenderActive; }
	void Set_MyChunk(int _num) { m_iMyChunk = _num; } //아이템 어떤 청크레이어에 생성할 지 필요

protected:
	HRESULT Ready_Components();
	bool m_bRenderActive = true;

	CVIBuffer_CubeInstance* m_pVIBufferCom = { nullptr };
	CCollider_Cube* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	int m_iMyChunk = 0;
	
public:
	static CBreakableCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

