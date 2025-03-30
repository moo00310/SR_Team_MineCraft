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
    m_pTargetPawn = static_cast<CPawn*>(m_pGameInstance->Get_LastObject(LEVEL_HERO, TEXT("Layer_Steve")));
    Safe_AddRef(m_pTargetPawn);

    return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
}

void CMonster::Update(_float fTimeDelta)
{
    // ���̶� �浹
    m_pRigidbodyCom->Update(fTimeDelta, COLLISION_BLOCK);

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
    m_pTransformCom->Chase(_float3(vTarget.x, 0.f, vTarget.z), _fTimeDelta, 1.0f);

}

HRESULT CMonster::Ready_BehaviorTree()
{
    // ��Ʈ ���: Selector (���� �߰��ϸ� ���󰡰�, �ƴϸ� ����)
    CSelectorNode* pRoot = new CSelectorNode(L"Root");

    // ���� �˻� ���: ���� �ִ��� Ȯ��
    CBTTask_DetectEnemy* pDetectEnemy = new CBTTask_DetectEnemy;

    // �ൿ���
    CBTTask_Chase* pChase = new CBTTask_Chase;
    CBTTask_Patrol* pPatrol = new CBTTask_Patrol;
    CBTTask_Attack* pAttack = new CBTTask_Attack;

    // ����, �߰�,
    CBTDistanceBranch* pDistanceBranch = new CBTDistanceBranch;
    pDistanceBranch->Set_Actions(pAttack, pChase, m_fAttackDistance);

    // ������
    CSequenceNode* pChaseSequence = new CSequenceNode(L"ChaseSequence");
    pChaseSequence->Add_Node(pDetectEnemy);
    pChaseSequence->Add_Node(pDistanceBranch);

    // ��Ʈ ��忡 �߰�
    pRoot->Add_Node(pChaseSequence);
    pRoot->Add_Node(pPatrol);

    // ���� Ʈ�� ����
    m_pBehaviorTree = pRoot;

    return S_OK;
}

HRESULT CMonster::Ready_Components()
{
    __super ::Ready_Components();

    // BT ����
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
