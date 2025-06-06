﻿#pragma once
// 부숴지는 큐브들의 부모가 되는 곳

#include "Client_Defines.h"
#include "Cube.h"
#include "Transform.h"
#include "ItemCube.h"
#include "ItemRect.h"
#include "ParticleEventManager.h"
#include "Warden.h"

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
	
	//아이템 어떤 청크레이어에 생성할 지 필요
	void Set_MyChunk(int _num) { m_iMyChunk = _num; } 
	// 인스턴싱 버퍼 업데이트
	void Set_InstanceBuffer(vector<D3DXVECTOR3> _objects, vector<_float> _brights) { m_pVIBufferCom->Update_InstanceBuffer(_objects, _brights); }
	void Set_InstanceBuffer(vector<D3DXVECTOR3> _objects, _float _bright) { m_pVIBufferCom->Update_InstanceBuffer(_objects, _bright); }

	// 충돌 큐브 생성 & 위치 설정
	void Set_BlockPositions(vector<_float3> position, ITEMNAME _name);
	_uint Get_PositionSize() { return static_cast<_uint>(m_vecPositions.size()); }

	// 충돌 활성화 & 비활성화 관리를 위함
	unordered_map<_int3, CCollider_Cube*>& Get_ColliderCube() { return m_Colliders; }
	vector<_float3> Get_Positions() { return m_vecPositions; }

	// 충돌할 청크만 활성화 시킴
	void Set_ChunkColliderActive(bool _b) { m_bChunkColliderActive = _b; }
	bool Get_ChunkColliderActive() { return m_bChunkColliderActive; }


	// 큐브 Hp 줄이기
	virtual void Attacked_Block(_float3 vPos, int attackDamage, _float fDeltaTime);

	//부수는 사운드
	virtual void PlaySound_Hit(_float3 vPos);

	// 큐브 HP 반환.
	float GetHP() const;

	//블럭 밝기 조절
	void Set_Bright();

	virtual HRESULT Delete_Cube(_float3 fPos);
	virtual HRESULT Create_Cube(_float3 fPos, _float3 _Dir);

	virtual void Play_Create_Sound(_float3 vPos);

	//블럭 경도 가지고 오는 코드
	_float Get_Hardness() { return m_fHardness; }

	HRESULT Ready_Components();

	ITEMNAME	Get_ItemName() { return m_itemName; }

protected:
	virtual HRESULT Drop_Item_OnDestroy(const _float3& fPos);
	virtual HRESULT Play_Destroy_Effect(const _float3& fPos);

protected:

	// 현재 청크를 충돌 매니저에 올릴지 
	bool m_bChunkColliderActive = false;


	CVIBuffer_CubeInstance* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	int m_iMyChunk = 0;
	vector<_float3> m_vecPositions;
	vector<_float> m_vecBrights;


	unordered_map<_int3, CCollider_Cube*> m_Colliders;

	ITEMNAME m_itemName{ ITEMNAME_END };

	// 워든 주소.
	CWarden* m_Warden = {nullptr};

	float m_fHp = 100;
	_float3 m_attackedBlockPos;
	int m_resetHpFrame = 0;

	//블럭 경도
	_float m_fHardness{ 0.f };

	_float m_fHitSoundDelayAcc{ 0.f };    // 누적 시간
	const _float m_fHitSoundDelayLimit{ 0.25f }; // 쿨타임 제한



public:
	static CBreakableCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

