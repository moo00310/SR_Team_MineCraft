#pragma once
// 부숴지는 큐브들의 부모가 되는 곳

#include "Client_Defines.h"
#include "Cube.h"
#include "Transform.h"
#include "UIDropItem.h"

BEGIN(Engine)
class CVIBuffer_Cube;
class CShader;
END

BEGIN(Client)	

class CItemCube : public CCube
{
public:
	typedef struct ItemPosition
	{
		_float3 position;
	}DESC;
public:
	CItemCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CItemCube(const CItemCube& Prototype);
	virtual ~CItemCube() = default;

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
	HRESULT Set_ItemTypeAndBindTexture(ITEMNAME _type);
	CRigidbody* Get_Rigidbody() { return m_pRigidbodyCom; }

	void Set_Bright(_float _y);
protected:
	HRESULT Ready_Components();

	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };
	CRigidbody* m_pRigidbodyCom = { nullptr };	
	CShader* m_pShaderCom = { nullptr };

	_float m_fUpDownTime{ 0 };
	_float m_fUpDownSpeed{ 0.f };

	ITEMNAME m_eItemName{ ITEMNAME_END };



	CTransform* m_pPlayerTransformCom{ nullptr };
	class  CMCTerrain* m_pTerrain{ nullptr };

private:
	//타임 저장
	_float		m_fTime{ 0.f };
	//생성되고 먹을 수 있는 타임
	const _float m_fCanGetTime{ 1.f };

	_float m_bright;
private:
	_float Compute_PlayerDistance();

	// UI 드랍 아이템 나오게한다.
	void OnDropItem(ITEMNAME _itemName);

public:
	static CItemCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END