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
public:
	CGameInstance* GetGameInstance();
	int GetFileCount();

	void ActivateChunkLayer(int chunkIndex, bool _b);

	//현재 활성화된 청크의 블럭을 추가하는 함수
	void Create_Cube(ITEMNAME eItemName, _float3 vPos, _float3 vDir);
private:
	HRESULT Ready_Layer_BackGround();

	//플레이어가 있는 청크에서만 충돌 매니저에 올려줌
	void SetColliderChunk();

	// 모든 청크 비활성화
	void OffAllChunkLayer();
	//플레이어 주변 3x3 청크만 활성화
	void GetPlayerChunk3x3();


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