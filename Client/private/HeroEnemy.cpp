#include "HeroEnemy.h"
#include "GameInstance.h"
#include "HeroCube.h"

#pragma region Action
#include "Check_Enemy_Node.h"
#include "Chase_Enemy_Node.h"
#include "Patrol_Node.h"
#pragma endregion


CHeroEnemy::CHeroEnemy(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CHeroEnemy::CHeroEnemy(const CHeroEnemy& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CHeroEnemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHeroEnemy::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(static_cast<_float>(rand() % 10), 0.5f, static_cast<_float>(rand() % 10)));

	Ready_BehaviorTree();

	return S_OK;
}

void CHeroEnemy::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, this);
}

void CHeroEnemy::Update(_float fTimeDelta)
{
	if (m_pBehaviorTree)
	{
		m_pBehaviorTree->Excute(this, fTimeDelta);
	}

	if (FAILED(m_pColliderCom->Update_ColliderBox()))
	{
		MSG_BOX("Update_ColliderBox()");
		return;
	}

	//_float fRange;
	
	m_bHit = m_pGameInstance->Collision_with_Group(COLLISION_PLAYER, this, CCollider_Manager::COLLSIION_BOX);

	/*if (!m_pTarget)
	{
		_bool isHit{ false };
		isHit =m_pGameInstance->Ray_Cast(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_State(CTransform::STATE_LOOK), 5.f, COLLISION_PLAYER, fRange, &m_pTarget);
		if (isHit)
		{
			CHeroCube* pHeroCube{ nullptr };
			pHeroCube = static_cast<CHeroCube*>(m_pTarget);
			CComponent* pCom{ nullptr };
			pCom = pHeroCube->Find_Component(TEXT("Com_Transform"));
			m_pTargetTransform = static_cast<CTransform*>(pCom);
		}
	}
	else
	{
		m_pTransformCom->LookAt(m_pTargetTransform->Get_State(CTransform::STATE_POSITION));
		m_pTransformCom->Chase(m_pTargetTransform->Get_State(CTransform::STATE_POSITION), fTimeDelta, 5.f);
	}*/

}

void CHeroEnemy::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CHeroEnemy::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (FAILED(m_pColliderCom->Render_ColliderBox(m_bHit)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHeroEnemy::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_Texture_BackGround"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CCollider_Cube::COLLRECTDESC Desc{}; //콜라이더 크기 설정
	Desc.fRadiusX = 1.f; Desc.fRadiusY = 1.f; Desc.fRadiusZ = 1.f;
	Desc.pTransformCom = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_CCollider_Cube"),
		TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHeroEnemy::Ready_BehaviorTree()
{
	// 루트 노드: Selector (적을 발견하면 따라가고, 아니면 순찰)
	CSelectorNode* pRoot = new CSelectorNode(L"Root");

	// 조건 검사 노드: 적이 있는지 확인
	CCheck_Enemy_Node* pCheckEnemy = new CCheck_Enemy_Node();

	// 행동 노드: 적을 따라가기
	CChase_Enemy_Node* pChaseEnemy = new CChase_Enemy_Node();

	// 행동 노드: 순찰하기
	CPatrol_Node* pPatrol = new CPatrol_Node();

	// 적이 있다면 따라가는 시퀀스
	CSequenceNode* pChaseSequence = new CSequenceNode(L"ChaseSequence");
	pChaseSequence->Add_Node(pCheckEnemy);
	pChaseSequence->Add_Node(pChaseEnemy);

	// 루트 노드에 추가
	pRoot->Add_Node(pChaseSequence);
	pRoot->Add_Node(pPatrol);

	// 최종 트리 설정
	m_pBehaviorTree = pRoot;

	return S_OK;
}

CHeroEnemy* CHeroEnemy::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHeroEnemy* pInstance = new CHeroEnemy(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHeroEnemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHeroEnemy::Clone(void* pArg)
{
	CHeroEnemy* pInstance = new CHeroEnemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CHeroEnemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHeroEnemy::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBehaviorTree);

}
