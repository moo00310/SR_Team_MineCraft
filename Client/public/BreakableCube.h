#pragma once
// �ν����� ť����� �θ� �Ǵ� ��

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
	
	//������ � ûũ���̾ ������ �� �ʿ�
	void Set_MyChunk(int _num) { m_iMyChunk = _num; } 
	// �ν��Ͻ� ���� ������Ʈ
	void Set_InstanceBuffer(vector<D3DXVECTOR3> _objects) { m_pVIBufferCom->Update_InstanceBuffer(_objects); }

	// �浹 ť�� ���� & ��ġ ����
	void Set_BlockPositions(vector<_float3> position);
	int Get_PositionSize() { return m_vecPositions.size(); }

	// �浹 Ȱ��ȭ & ��Ȱ��ȭ ������ ����
	vector<CCollider_Cube*> Get_ColliderCube() { return m_Colliders; }
	vector<_float3> Get_Positions() { return m_vecPositions; }

	// �浹�� ûũ�� Ȱ��ȭ ��Ŵ
	void Set_ChunkColliderActive(bool _b) { m_bChunkColliderActive = _b; }
	bool Get_ChunkColliderActive() { return m_bChunkColliderActive; }

	//�浹�� ť�� ����
	virtual HRESULT Delete_Cube(_float3 fPos);

	//�� ��� ����
	void Set_Bright(float _f) { m_fBright = _f; }
protected:
	HRESULT Ready_Components();

	// ���� ûũ�� �浹 �Ŵ����� �ø��� 
	bool m_bChunkColliderActive = false;

	CVIBuffer_CubeInstance* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	int m_iMyChunk = 0;
	vector<_float3> m_vecPositions;
	vector<CCollider_Cube*> m_Colliders;

	float m_fBright=1;
public:
	static CBreakableCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

