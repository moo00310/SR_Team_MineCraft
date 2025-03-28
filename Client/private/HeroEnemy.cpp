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

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 30.f, 0.5f, 30.f });

	Ready_BehaviorTree();

	return S_OK;
}

void CHeroEnemy::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(COLLISION_BLOCK, this);

	if (m_pGameInstance->Key_Down('K'))
	{
		Destroy();
	}
}

void CHeroEnemy::Update(_float fTimeDelta)
{
	if (m_pBehaviorTree)
	{
		m_pBehaviorTree->Excute(this, fTimeDelta);
	}

	if (FAILED(m_pColliderCom->Update_Collider()))
	{
		MSG_BOX("Update_Collider()");
		return;
	}

	//_float fRange;
	
	if (m_pGameInstance->Collision_Check_with_Group(COLLISION_PLAYER, m_pColliderCom, CCollider_Manager::COLLSIION_CUBE))
	{
		m_bHit = true;
	}


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

	if (FAILED(m_pColliderCom->Render_Collider(m_bHit)))
		return E_FAIL;

	return S_OK;
}

CTransform* CHeroEnemy::Get_Transform()
{
	return m_pTransformCom;
}

CTransform* CHeroEnemy::Get_Target_Transform()
{
	return m_pTargetTransform;
}

CGameObject*& CHeroEnemy::Get_Target()
{
	return m_pTarget;
}

void CHeroEnemy::Set_Target(CGameObject* pGameObject)
{
	m_pTarget = pGameObject;
}

void CHeroEnemy::Set_Target_Transform(CTransform* pTransform)
{
	m_pTargetTransform = pTransform;
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
	CCollider_Cube::COLLCUBE_DESC Desc{}; //�ݶ��̴� ũ�� ����
	Desc.vRadius = { 1.f, 1.f, 1.f };
	Desc.pTransformCom = m_pTransformCom;
	Desc.pOwner = this;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHeroEnemy::Ready_BehaviorTree()
{
	// ��Ʈ ���: Selector (���� �߰��ϸ� ���󰡰�, �ƴϸ� ����)
	CSelectorNode* pRoot = new CSelectorNode(L"Root");

	// ���� �˻� ���: ���� �ִ��� Ȯ��
	CCheck_Enemy_Node* pCheckEnemy = new CCheck_Enemy_Node;

	// �ൿ ���: ���� ���󰡱�
	CChase_Enemy_Node* pChaseEnemy = new CChase_Enemy_Node;

	// �ൿ ���: �����ϱ�
	CPatrol_Node* pPatrol = new CPatrol_Node();

	// ���� �ִٸ� ���󰡴� ������
	CSequenceNode* pChaseSequence = new CSequenceNode(L"ChaseSequence");
	pChaseSequence->Add_Node(pCheckEnemy);
	pChaseSequence->Add_Node(pChaseEnemy);

	// ��Ʈ ��忡 �߰�
	pRoot->Add_Node(pChaseSequence);
	pRoot->Add_Node(pPatrol);

	// ���� Ʈ�� ����
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
