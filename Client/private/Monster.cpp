#include "Monster.h"

#include "BTTask_DetectEnemy.h"
#include "BTTask_Chase.h"
#include "BTTask_Patrol.h"
#include "BTDecorator_IsTargetNear.h"


CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CMonster::CMonster(const CMonster& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CMonster::Initialize(void* pArg)
{
	Ready_Components();
	Ready_BehaviorTree();
	return E_NOTIMPL;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
}

void CMonster::Update(_float fTimeDelta)
{
	if (m_pBehaviorTree)
	{
		m_pBehaviorTree->Excute(this, fTimeDelta);
	}
}

void CMonster::Late_Update(_float fTimeDelta)
{
}

HRESULT CMonster::Render()
{
	return E_NOTIMPL;
}

//CGameObject*& CMonster::Get_Target()
//{
//	return nullptr;
//}

CGameObject* CMonster::Get_Target()
{
	return m_pTarget;
}

void CMonster::Set_Target(CGameObject* pGameObject)
{
    m_pTarget = pGameObject;
}

CTransform* CMonster::Get_Transform()
{
	return m_pTransformCom;
}

HRESULT CMonster::Ready_BehaviorTree()
{
    // 루트 노드: Selector (적을 발견하면 따라가고, 아니면 순찰)
    CSelectorNode* pRoot = new CSelectorNode(L"Root");

    // 조건 검사 노드: 적이 있는지 확인
    CBTTask_DetectEnemy* pDetectEnemy = new CBTTask_DetectEnemy;

    // 행동 노드: 적을 따라가기
    CBTTask_Chase* pChase = new CBTTask_Chase;

    // 행동 노드: 순찰하기
    CBTTask_Patrol* pPatrol = new CBTTask_Patrol;

    // 거리 제한 데코레이터 추가 (500 이상이면 FAIL 반환)
    CBTDecorator_IsTargetNear* pCheckDistance = new CBTDecorator_IsTargetNear;
    pCheckDistance->Set_DecoratorNodes(pChase, nullptr);

    // 적이 있다면 따라가는 시퀀스
    CSequenceNode* pChaseSequence = new CSequenceNode(L"ChaseSequence");
    pChaseSequence->Add_Node(pDetectEnemy);
    pChaseSequence->Add_Node(pCheckDistance); // 거리 체크 추가

    // 루트 노드에 추가
    pRoot->Add_Node(pChaseSequence);
    pRoot->Add_Node(pPatrol);

    // 최종 트리 설정
    m_pBehaviorTree = pRoot;

    return S_OK;
}

HRESULT CMonster::Ready_Components()
{
	return S_OK;
}

CGameObject* CMonster::Clone(void* pArg)
{
	return nullptr;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pBehaviorTree);
	Safe_Release(m_pTransformCom);
}
