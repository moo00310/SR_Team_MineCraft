#pragma once
// 부숴지는 큐브들의 부모가 되는 곳

#include "Client_Defines.h"
#include "Cube.h"
#include "Transform.h"
#include "ItemCube.h"
#include "ItemRect.h"


BEGIN(Engine)
class CVIBuffer_Rect3DInstance;
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
	void Set_MyChunk(int _num) { m_iMyChunk = _num; } //아이템 어떤 청크레이어에 생성할 지 필요

	// 충돌할 청크만 활성화 시킴
	void Set_ChunkColliderActive(bool _b) { m_bChunkColliderActive = _b; }
	bool Get_ChunkColliderActive() { return m_bChunkColliderActive; }

	// 인스턴싱 버퍼 업데이트
	void Set_InstanceBuffer(vector<D3DXVECTOR3> _objects, vector<_float> _brights) { m_pVIBufferCom->Update_InstanceBuffer(_objects, _brights); }
	void Set_InstanceBuffer(vector<D3DXVECTOR3> _objects, _float _bright) { m_pVIBufferCom->Update_InstanceBuffer(_objects, _bright); }

	void Set_BlockPositions(vector<_float3> position, ITEMNAME _name);

	//충돌된 큐브 삭제
	virtual HRESULT Delete_Cube(_float3 fPos);

	//블럭 밝기 조절
	void Set_Bright();

	unordered_map<_int3, CCollider_Cube*>& Get_ColliderRect() { return m_Colliders; }

protected:
	virtual HRESULT Drop_Item_OnDestroy(const _float3& fPos) { return S_OK; }
	virtual HRESULT Play_Destroy_Effect(const _float3& fPos) { return S_OK; }

protected:
	HRESULT Ready_Components();

	CVIBuffer_Rect3DInstance* m_pVIBufferCom = { nullptr};
	CShader* m_pShaderCom = { nullptr };

	int m_iMyChunk = 0;
	vector<_float3> m_vecPositions;
	vector<_float> m_vecBrights;
	unordered_map<_int3, CCollider_Cube*> m_Colliders;

	bool m_bChunkColliderActive = false;

	ITEMNAME m_itemName{ ITEMNAME_END };

public:
	static CBreakableRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

