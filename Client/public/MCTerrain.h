#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "BreakableCube.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CMCTerrain final : public CGameObject
{
private:
	CMCTerrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMCTerrain(const CMCTerrain& Prototype);
	virtual ~CMCTerrain() = default;

public:	
	virtual HRESULT Initialize_Prototype()override;	
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

#pragma region 청크 콜라이더 활성화 관련
public:
	//가까운 청크중에서 특정 거리만큼 가까운 블럭의 콜라이더를 활성화 시키고 그 콜라이더들을 반환하는 함수
	list<CCollider*> Active_Near_Chunk_Colliders(_float3 vPos, _float fDistSq); //이건 두꺼운 애들 (CPawn) ----> 1 ~ 4 청크
	list<CCollider*> Active_Current_Chunk_Colliders(_float3 vPos, _float fDistSq);//아이템 처럼 충돌이 얇은 애들(CItemCube, CItemRect) ----> 1청크
private:
	//근접한 청크의 인덱스를 가져오는 함수(보통 1, 많으면 4개)
	set<_int>& Compute_Near_Chunk_Indexies(_float3 vPos);
	//현재 위치로 청크 인덱스를 알아오는 함수
	_int Compute_ChunkIndex(_float3 vPos);
#pragma endregion

public:
	CGameInstance* GetGameInstance();
	int GetFileCount();
	void ActivateChunkLayer(int chunkIndex, bool _b);
	//현재 활성화된 청크의 블럭을 추가하는 함수
	void Create_Cube(_float3 vMyPos, ITEMNAME eItemName, _float3 vPos, _float3 vDir);
	// 현재 청크에 스폰 가능한 위치를 가져오는 함수
	const vector<_float3>& Get_SpwanAble();
private:

	HRESULT Ready_Layer_BackGround();

	//플레이어가 있는 청크에서만 충돌 매니저에 올려줌
	void SetColliderChunk();

	// 모든 청크 비활성화
	void OffAllChunkLayer();

	//플레이어 주변 3x3 청크만 활성화
	void GetPlayerChunk3x3();

	set<_int> m_ActiveChunkIndexies; // 활성화 된 청크 인덱스 모음

private:
	map<int, vector<_float3>> m_SpawnPos;
	vector<_float3> m_vecMerged;


private:
	int m_iChunkCount = 0; // 청크 개수 
	int m_prePlayerChunk = 0; //이전 청크
	int m_currentPlayerChunk = 0; //현재 청크
	bool prevF1State = false;
	bool prevF2State = false;
public:
	static CMCTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
	
};

END