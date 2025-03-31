#include "Monster.h"

#include "BTTask_DetectEnemy.h"
#include "BTTask_Chase.h"
#include "BTTask_Patrol.h"
#include "BTDecorator_IsTargetNear.h"
#include "BTTask_Attack.h"
#include "BTDistanceBranch.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CPawn{ pGraphic_Device }
{
}

CMonster::CMonster(const CMonster& Prototype)
	:CPawn(Prototype)
{
}

HRESULT CMonster::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
    m_pTargetPawn = static_cast<CPawn*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve")));
    Safe_AddRef(m_pTargetPawn);

    return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
    if (m_Hp <= 0.f)
    {
        isDead = true;
        m_eCurAnim = DEAD;
    }

}

void CMonster::Update(_float fTimeDelta)
{
    //플레이어랑 멀면 여기 비활성화
    _float3 vTargetPos{ static_cast<CTransform*>(m_pTargetPawn->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION) };
    _float3 vDiff{ vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)};
    _float fLengthSq{ D3DXVec3LengthSq(&vDiff) };
    if (fLengthSq > 500.f)
    {
		//타겟과 너무 멀다면-> 비활성화
        return;
    }

    if (m_pBehaviorTree && !isDead)
    {
        m_pBehaviorTree->Excute(this, fTimeDelta);
    }

    // 땅 충돌 + 중력 처리
    m_pRigidbodyCom->Update(fTimeDelta, COLLISION_BLOCK);
}

void CMonster::Late_Update(_float fTimeDelta)
{
  
}

HRESULT CMonster::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    for (auto buffer : m_pVIBufferComs)
    {
        if (FAILED(buffer->Bind_WorldMatrix()))
            return E_FAIL;

        if (FAILED(buffer->Bind_Buffers()))
            return E_FAIL;

        if (FAILED(buffer->Render()))
            return E_FAIL;
    }

	return S_OK;
}

float CMonster::Comput_Distance()
{
    _float3 vMonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _float3 vTargetPos = m_pTargetPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION);

    _float3 vDiff = vTargetPos - vMonsterPos;

    return D3DXVec3Length(&vDiff);
}

void CMonster::Chase_Player(float _fTimeDelta)
{
    _float3 vTarget =  m_pTargetPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION);
    m_pTransformCom->LookAt_XZ(vTarget);
    m_pTransformCom->Chase(_float3(vTarget.x, vTarget.y, vTarget.z), _fTimeDelta, 1.0f);

    //움직일라 하는데 속도가 안난다 점프함 ㅋㅋ
    if (D3DXVec3LengthSq(&m_pRigidbodyCom->Get_Velocity()) < 0.1f)
    {
		m_pRigidbodyCom->Jump(7.f);
    }
}


void CMonster::Knock_back(const _float3& vforce)
{

    _float3 temp = {};
    D3DXVec3Normalize(&temp, &vforce);
    temp *= 3.f;
    temp.y = 4.f;

    m_pRigidbodyCom->Knock_back(temp);

    _float3 vTarget = m_pTargetPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION);
    m_pTransformCom->LookAt_XZ(vTarget);

}

HRESULT CMonster::Ready_BehaviorTree()
{
    // 루트 노드: Selector (적을 발견하면 따라가고, 아니면 순찰)
    CSelectorNode* pRoot = new CSelectorNode(L"Root");

    // 조건 검사 노드: 적이 있는지 확인
    CBTTask_DetectEnemy* pDetectEnemy = new CBTTask_DetectEnemy;

    // 행동노드
    CBTTask_Chase* pChase = new CBTTask_Chase;
    CBTTask_Patrol* pPatrol = new CBTTask_Patrol;
    CBTTask_Attack* pAttack = new CBTTask_Attack;

    // 공격, 추격,
    CBTDistanceBranch* pDistanceBranch = new CBTDistanceBranch;
    pDistanceBranch->Set_Actions(pAttack, pChase, m_fAttackDistance);

    // 시퀀스
    CSequenceNode* pChaseSequence = new CSequenceNode(L"ChaseSequence");
    pChaseSequence->Add_Node(pDetectEnemy);
    pChaseSequence->Add_Node(pDistanceBranch);

    // 루트 노드에 추가
    pRoot->Add_Node(pChaseSequence);
    pRoot->Add_Node(pPatrol);

    // 최종 트리 설정
    m_pBehaviorTree = pRoot;

    return S_OK;
}

HRESULT CMonster::Ready_Components()
{
    __super ::Ready_Components();

    // BT 연결
    if(FAILED(Ready_BehaviorTree()))
        return E_FAIL;

    return S_OK;
}

void CMonster::Free()
{
    __super::Free();

    Safe_Release(m_pTargetPawn);
    Safe_Release(m_pBehaviorTree);
}
