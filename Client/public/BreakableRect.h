#pragma once
// �ν����� ť����� �θ� �Ǵ� ��

#include "Client_Defines.h"
#include "Cube.h"
#include "Transform.h"
#include "ItemCube.h"
#include "ItemRect.h"


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
	void Set_MyChunk(int _num) { m_iMyChunk = _num; } //������ � ûũ���̾ ������ �� �ʿ�

	// �浹�� ûũ�� Ȱ��ȭ ��Ŵ
	void Set_ChunkColliderActive(bool _b) { m_bChunkColliderActive = _b; }
	bool Get_ChunkColliderActive() { return m_bChunkColliderActive; }
protected:
	HRESULT Ready_Components();
	CVIBuffer_Rect3D* m_pVIBufferCom = { nullptr};
	CCollider_Cube* m_pColliderCom = { nullptr };
	
	int m_iMyChunk = 0;

	// ���� ûũ�� �浹 �Ŵ����� �ø��� 
	bool m_bChunkColliderActive = false;

public:
	static CBreakableRect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

