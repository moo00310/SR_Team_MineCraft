#include "Monster.h"

#include "BTTask_DetectEnemy.h"
#include "BTTask_Chase.h"
#include "BTTask_Patrol.h"
#include "BTDecorator_IsTargetNear.h"
#include "BTTask_Attack.h"
#include "BTDistanceBranch.h"

#include <iostream>


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
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
    return S_OK;
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
    pDistanceBranch->Set_Actions(pAttack, pChase);

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
    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 1.5f, D3DXToRadian(30.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    // �� + �ִϸ��̼�
    CSkeletalAnimator::DESC DescSekel = { m_pVIBufferComs };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SkeletalAnimator"),
        TEXT("m_pSkeletalAnimatorCom"), reinterpret_cast<CComponent**>(&m_skelAnime), &DescSekel)))
        return E_FAIL;

    // BT ����
    if(FAILED(Ready_BehaviorTree()))
        return E_FAIL;

    return S_OK;
}

void CMonster::Free()
{
	__super::Free();
	Safe_Release(m_pBehaviorTree);
	Safe_Release(m_pTransformCom);
    Safe_Release(m_skelAnime);
    for (auto& buffer : m_pVIBufferComs)
        Safe_Release(buffer);
}
