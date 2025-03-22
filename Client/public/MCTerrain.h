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
	void CheckRenderLayerObjects();
private:
	HRESULT Ready_Layer_BackGround();
	// ��� ��ü Render

	// �� ���� Render
	void RenderWithoutStone();
	// ��� ûũ ��Ȱ��ȭ
	void OffAllChunkLayer();

	//�÷��̾� ��� ûũ�� �ִ��� ������ �Լ�
	void GetPlayerChunk3x3();
	void GetPlayerChunk();


	int m_iChunkCount = 0;
	bool prevF1State = false;
	bool prevF2State = false;
public:
	static CMCTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
	
};

END