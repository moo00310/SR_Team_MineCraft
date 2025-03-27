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
	void CheckColliderActive();
private:
	HRESULT Ready_Layer_BackGround();

	// 모든 청크 비활성화
	void OffAllChunkLayer();

	//플레이어 어디 청크에 있는지 들고오는 함수
	void GetPlayerChunk3x3();
	void GetPlayerChunk();

	//플레이어가 있는 청크에서만 충돌 매니저에 올려줌
	void SetColliderChunk();

	int m_iChunkCount = 0;
	bool prevF1State = false;
	bool prevF2State = false;
public:
	static CMCTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
	
};

END