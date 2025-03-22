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
    // ��Ʈ ���: Selector (���� �߰��ϸ� ���󰡰�, �ƴϸ� ����)
    CSelectorNode* pRoot = new CSelectorNode(L"Root");

    // ���� �˻� ���: ���� �ִ��� Ȯ��
    CBTTask_DetectEnemy* pDetectEnemy = new CBTTask_DetectEnemy;

    // �ൿ ���: ���� ���󰡱�
    CBTTask_Chase* pChase = new CBTTask_Chase;

    // �ൿ ���: �����ϱ�
    CBTTask_Patrol* pPatrol = new CBTTask_Patrol;

    // �Ÿ� ���� ���ڷ����� �߰� (500 �̻��̸� FAIL ��ȯ)
    CBTDecorator_IsTargetNear* pCheckDistance = new CBTDecorator_IsTargetNear;
    pCheckDistance->Set_DecoratorNodes(pChase, nullptr);

    // ���� �ִٸ� ���󰡴� ������
    CSequenceNode* pChaseSequence = new CSequenceNode(L"ChaseSequence");
    pChaseSequence->Add_Node(pDetectEnemy);
    pChaseSequence->Add_Node(pCheckDistance); // �Ÿ� üũ �߰�

    // ��Ʈ ��忡 �߰�
    pRoot->Add_Node(pChaseSequence);
    pRoot->Add_Node(pPatrol);

    // ���� Ʈ�� ����
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
