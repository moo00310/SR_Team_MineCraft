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

	//���� Ȱ��ȭ�� ûũ�� ���� �߰��ϴ� �Լ�
	void Create_Cube(ITEMNAME eItemName, _float3 vPos, _float3 vDir);
private:
	HRESULT Ready_Layer_BackGround();

	//�÷��̾ �ִ� ûũ������ �浹 �Ŵ����� �÷���
	void SetColliderChunk();

	// ��� ûũ ��Ȱ��ȭ
	void OffAllChunkLayer();
	//�÷��̾� �ֺ� 3x3 ûũ�� Ȱ��ȭ
	void GetPlayerChunk3x3();


	int m_iChunkCount = 0; // ûũ ���� 

	int m_prePlayerChunk = 0; //���� ûũ
	int m_currentPlayerChunk = 0; //���� ûũ
	bool prevF1State = false;
	bool prevF2State = false;
public:
	static CMCTerrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
	
};

END