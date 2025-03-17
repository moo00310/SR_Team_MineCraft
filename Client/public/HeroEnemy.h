#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Behavior_Tree.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CCollider_Cube;
END

BEGIN(Client)

class CHeroEnemy final : public CGameObject
{
	//프렌드 하기 vs Get()Set() 만들기
	friend class CCheck_Enemy_Node;
	friend class CChase_Enemy_Node;

private:
	CHeroEnemy(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHeroEnemy(const CHeroEnemy& Prototype);
	virtual ~CHeroEnemy() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
private:
	_bool			m_bHit{ false };
	CGameObject*	m_pTarget{ nullptr };
	CTransform*		m_pTargetTransform{ nullptr };
private:
	CTexture*		m_pTextureCom = { nullptr };
	CTransform*		m_pTransformCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };
	CCollider_Cube* m_pColliderCom = { nullptr };

	CNode*			m_pBehaviorTree{ nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_BehaviorTree();

public:
	static CHeroEnemy* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END