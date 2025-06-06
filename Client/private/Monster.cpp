﻿#include "Monster.h"

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
    __super::Initialize(pArg);

    m_pTargetPawn = static_cast<CPawn*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve")));
    Safe_AddRef(m_pTargetPawn);
    
    for (size_t i = 0; i < ANIM_END; i++)
    {
        m_bAnimEnd[i] = false;
    }

    m_sound = 0.5f;

    return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
    if (!isDead)
        m_pGameInstance->Add_CollisionGroup(COLLISION_MONSTER, this);

    if (m_bGetHit) 
    {
        m_iGetHitFrame++;
        if (m_iGetHitFrame > 15 && (m_eCurAnim != DEAD)) {
            m_bGetHit = false;
            m_iGetHitFrame = 0;
            m_eColor = RENDERORIGIN;
        }
    }
}

void CMonster::Update(_float fTimeDelta)
{
	
    m_TargetPos = m_pTargetPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION);

    //_float fDist = Comput_Distance();
    //if (fDist > 30.f)
    //{
    //    //타겟과 너무 멀다면-> 비활성화
    //    return;
    //}
    //else if (fDist > 10.f && !m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.5f))
    //{
    //    //타겟과 적당히 멀고, 눈에 안보이면 비활성화
    //    return;
    //}

    if (m_pBehaviorTree && !isDead)
    {
        m_pBehaviorTree->Excute(this, fTimeDelta);
    }

    __super::Update(fTimeDelta);

}

void CMonster::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.5f))
    {
        m_skelAnime->Update_RootBone(*m_pTransformCom->Get_WorldMatrix());
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }

}

HRESULT CMonster::Render()
{
    __super::Render();

	return S_OK;
}

float CMonster::Comput_Distance()
{
    _float3 vMonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _float3 vDiff = m_TargetPos - vMonsterPos;

    return D3DXVec3Length(&vDiff);
}

void CMonster::Chase_Player(float _fTimeDelta)
{

    if (m_MonsterType != MT_WARDEN)
    {
        m_pTransformCom->LookAt_XZ(m_TargetPos);

        if (!m_pRigidbodyCom->Get_isKnockBack())
        {
            m_pRigidbodyCom->Chase(m_TargetPos, 2.f);
        }
    }
    else
    {
        LookAtPlayer(_fTimeDelta);
        m_pTransformCom->Go_Direction(m_skelAnime->GetBoneWorldMatrix(2).Get_State(Matrix::STATE_LOOK), _fTimeDelta);
    }
    
    //전 프레임 상태가 공격 이었으면 무조건 속도 안났을 테니 점프 하지 못하게 해야함
    //움직일라 하는데 속도가 안난다 점프함 ㅋㅋ
    if (D3DXVec3LengthSq(&m_pRigidbodyCom->Get_Velocity()) < 0.2f && (m_eCurAnim == WALK || m_eCurAnim == RUN))
    {
		m_pRigidbodyCom->Jump(7.0f);
    }
}

void CMonster::LookAtPlayer(float _fTimeDelta)
{
    _float3 vTargetPos = m_TargetPos;
    vTargetPos.y += 1.f;

    if(m_eCurAnim == FIND)
        m_skelAnime->LookAt_Anim(vTargetPos, 2);
    else
        m_skelAnime->LookAt(vTargetPos, 2);

    float fAngle = m_skelAnime->RotateRootByNeckDelta(2, 0, _fTimeDelta);
    m_pTransformCom->TurnByAngle(_float3(0.f, 1.f, 0.f), fAngle);
}

void CMonster::Init_BlackBoard()
{
    m_isFind = { false };
    m_ChaseTime = { 0.f };
    m_IdleTime = { 0.f };
    isAttack = { false };

    for (size_t i = 0; i < ANIM_END; i++)
    {
        m_bAnimEnd[i] = false;
    }
}

bool CMonster::GetAnimEnd(int type)
{
    return m_bAnimEnd[type];
}

void CMonster::Knock_back(const _float3& vforce)
{
    m_eColor = RENDERATTACKED;
    m_bGetHit = true;

    //_float3 temp = {};
    //D3DXVec3Normalize(&temp, &vforce);
    //temp *= 3.f;     temp.y = 4.f;

    m_pRigidbodyCom->Knock_back(vforce);
    m_pTransformCom->LookAt_XZ(m_TargetPos);

    int random = rand() % 10;
    switch (m_MonsterType)
    {
    case Client::CMonster::MT_Zombie:
        if (random < 5) {
            m_pGameInstance->Play_Sound(TEXT("Zombie_Hurt1"), SOUND_HIT, this, m_sound, m_TargetPos);
        }
        else {
            m_pGameInstance->Play_Sound(TEXT("Zombie_Hurt2"), SOUND_HIT, this, m_sound, m_TargetPos);
        }
        break;
    case Client::CMonster::MT_Creeper:
        if (random < 5) {
            m_pGameInstance->Play_Sound(TEXT("Creeper_Hurt1"), SOUND_HIT, this, m_sound, m_TargetPos);
        }
        else {
            m_pGameInstance->Play_Sound(TEXT("Creeper_Hurt2"), SOUND_HIT, this, m_sound, m_TargetPos);
        }
        break;

    case Client::CMonster::MT_WARDEN:
        if (random < 5) {
            m_pGameInstance->Play_Sound(TEXT("hurt_1"), SOUND_HIT, this, m_sound, m_TargetPos);
        }
        else {
            m_pGameInstance->Play_Sound(TEXT("hurt_2"), SOUND_HIT, this, m_sound, m_TargetPos);
        }
        break;
    case Client::CMonster::MT_END:
        break;
    default:
        break;
    }

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

void CMonster::OnPushPool()
{
    m_eCurAnim = IDLE;
    m_isFind = false;
    m_bGetHit = false;
    m_iGetHitFrame = 0;
    m_Hp = m_MaxHp;
    m_eColor = RENDERORIGIN;
    isDead = false;
    m_skelAnime->InitBone();
}

void CMonster::Free()
{
    __super::Free();

    Safe_Release(m_pTargetPawn);
    Safe_Release(m_pBehaviorTree);
}