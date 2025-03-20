#include "Steve.h"

#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include <iostream>

CSteve::CSteve(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
	for (int i = 0; i < 6; i++)
	{
		m_pVIBufferCom[i] = nullptr;
	}
}

CSteve::CSteve(const CSteve& Prototype)
	: CGameObject(Prototype)
{
	for (int i = 0; i < 6; i++)
	{
		m_pVIBufferCom[i] = nullptr;
	}
}

HRESULT CSteve::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSteve::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSteve::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(COLLISION_PLAYER, this);
	Move(fTimeDelta);
	Turn(fTimeDelta);
	m_pRigidbodyCom->Update(fTimeDelta, COLLISION_BLOCK);
}

void CSteve::Update(_float fTimeDelta)
{
	if (FAILED(m_pCollider_CubeCom->Update_ColliderBox()))
	{
		MSG_BOX("Update_ColliderBox()");
		return;
	}

	//CGameObject* pGameObject;
	//_float fDist;
	//m_pGameInstance->Ray_Cast
	//(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
	//	m_pTransformCom->Get_State(CTransform::STATE_LOOK),
	//	10.f,
	//	COLLISION_PLAYER,
	//	fDist,
	//	&pGameObject
	//);
}

void CSteve::Late_Update(_float fTimeDelta)
{
	// ���� ��Ʈ������ ���� Ʈ���������� ������Ʈ
	vecBones[0].transform = *(m_pTransformCom->Get_WorldMatrix());

	// ���� ����� �Ž� ������Ʈ
	Ready_Mesh();


	if (m_pGameInstance->Key_Down(VK_F5))
	{
		m_bisTPS *= -1;
	}
	if (m_bisTPS > 0)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
			return;
	}
}
		

HRESULT CSteve::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	/*if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;*/
	for (int i = 0; i < 6; i++)
	{
		if (FAILED(m_pVIBufferCom[i]->Bind_WorldMatrix()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[i]->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom[i]->Render()))
			return E_FAIL;
	}


	if (FAILED(m_pCollider_CubeCom->Render_ColliderBox(false)))
		return E_FAIL;

	return S_OK;
}

void CSteve::SetPos(_float3 v3)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, v3);
}

_float3 CSteve::GetPos()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION);
}

void CSteve::Move(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Pressing('W'))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);

		if (Comput > 20)
			flag *= -1;
		if (Comput < -20)
			flag *= -1;

		vecBones[3].transform.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(1.5f * flag));
		vecBones[4].transform.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-1.5f * flag));
		vecBones[5].transform.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-1.5f * flag));
		vecBones[6].transform.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(1.5f * flag));
		Comput += 1.5f * flag;
	}
	if (m_pGameInstance->Key_Pressing('S'))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing('A'))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
		//m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);
	}
	if (m_pGameInstance->Key_Pressing('D'))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
		//m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}

	if (m_pGameInstance->Key_Pressing(VK_SPACE))
	{
		m_pRigidbodyCom->Jump();
	}
}

void CSteve::Turn(_float fTimeDelta)
{
	// â�� Ȱ��ȭ ���°� �ƴ� ��� ���콺 �Է��� ����
	if (!(GetForegroundWindow() == g_hWnd))
		return;

	// ȭ�� �߾� ��ǥ ���
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

	// ���� ���콺 ��ǥ ��������
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	// === ���콺�� â ���ο� �ִ��� Ȯ�� ===
	if (ptMouse.x < 0 || ptMouse.x >= rc.right || ptMouse.y < 0 || ptMouse.y >= rc.bottom)
		return;

	// ���콺 �̵��� ��� (�߾� ����)
	_int iMouseMoveX = ptMouse.x - ptCenter.x;

	// ī�޶��� yaw ���� ����Ͽ� ��Ƽ���� ȸ������ ����
	m_pTransformCom->Turn({ 0.f, 1.f, 0.f }, fTimeDelta * iMouseMoveX * m_fMouseSensor);

	// ��Ƽ���� ���� ��Ʈ������ ������
	const _float4x4* pWorldMatrix = m_pTransformCom->Get_WorldMatrix();

	// 'Look' ���͸� ��� ����, ���� ��Ʈ�������� ���� ���͸� ����
	_float3 vLook = { pWorldMatrix->_31, pWorldMatrix->_32, pWorldMatrix->_33 };
	_float3 vUp = { pWorldMatrix->_21, pWorldMatrix->_22, pWorldMatrix->_23 };

	// 'Right' ���� ���: Look�� Up�� ����
	_float3 vRight;
	D3DXVec3Cross(&vRight, &vUp, &vLook);

	// ���콺 �̵��� ���� ȸ���� ����
	_float fYaw = iMouseMoveX * fTimeDelta * m_fMouseSensor;  // ȸ�� ���� ��� (yaw)

	// ȸ�� ��� ���� (Y�� ���� ȸ��)
	_float4x4 matRotation;
	D3DXMatrixRotationAxis(&matRotation, &vUp, fYaw);  // Y�� �������� ȸ��

	// ���� ���� ��Ʈ������ ȸ�� ��ķ� ����
	_float4x4 matNewWorld = *pWorldMatrix;
	matNewWorld = matRotation * matNewWorld;

	// ���ο� ���� ��Ʈ������ ����
	m_pTransformCom->Set_Matrix(matNewWorld);

	//// ���콺�� �߾����� �̵�
	//ClientToScreen(g_hWnd, &ptCenter);
	//SetCursorPos(ptCenter.x, ptCenter.y);
}

HRESULT CSteve::Ready_Components()
{
	// ��Ƽ�� �ؽ�ó
  /* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Steve"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_VIBuffer */

	// �Ӹ�
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Head"),
		TEXT("m_pVIBufferCom_Head"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[0]))))
		return E_FAIL;
	
	// ����
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Body"),
		TEXT("m_pVIBufferCom_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[1]))))
		return E_FAIL;

	// �ٸ�
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Foot_R"),
		TEXT("m_pVIBufferCom_Foot_R"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[2]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Foot_L"),
		TEXT("m_pVIBufferCom_Foot_L"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[3]))))
		return E_FAIL;

	// ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Arm_R"),
		TEXT("m_pVIBufferCom_Arm_R"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[4]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Arm_L"),
		TEXT("m_pVIBufferCom_Arm_L"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[5]))))
		return E_FAIL;


	if (FAILED(Ready_Bone()))
		return E_FAIL;
	if (FAILED(Ready_Mesh()))
		return E_FAIL;

	//�ݶ��̴�
	/* For.Com_Collider */
	CCollider_Cube::COLLCUBE_DESC Desc{}; //�ݶ��̴� ũ�� ����
	Desc.fRadiusX = 0.5f; Desc.fRadiusY = 1.f; Desc.fRadiusZ = 0.5;
	Desc.fOffSetY = 1.f;
	Desc.pTransformCom = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pCollider_CubeCom), &Desc)))
		return E_FAIL;

	//������ٵ�
	/* For.Com_Rigidbody */
	CRigidbody::RIGIDBODY_DESC	RigidbodyDesc{ m_pTransformCom, m_pCollider_CubeCom, 1.f };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Rigidbody"),
		TEXT("Com_Rigidbody"), reinterpret_cast<CComponent**>(&m_pRigidbodyCom), &RigidbodyDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CSteve::Ready_Bone()
{
	BONE bone[7] =
	{
		 { "Root"   ,0, -1,  MAtrixTranslation(0.f,              0.f,					0.f) },
		 { "Pelvis" ,1,  0,  MAtrixTranslation(0.f,              12.f / 16.f,			0.f) },
		 { "Neck"   ,2,  1,  MAtrixTranslation(0.f,              12.f / 16.f,			0.f) },
		 { "Leg_R"	,3,  1,  MAtrixTranslation(2.f / 16.f,		  0,					0.f) },
		 { "Leg_L"  ,4,  1,  MAtrixTranslation(-2.f / 16.f,       0,					0.f) },
		 { "Arm_R"  ,5,  1,  MAtrixTranslation(6.f / 16.f,        12.f / 16.f,			0.f) },
		 { "Arm_L"  ,6,  1,  MAtrixTranslation(-6.f / 16,         12.f / 16.f,			0.f) },
	};

	for (int i = 0; i < 7; i++)
	{
		vecBones.push_back(bone[i]);
	}

	return S_OK;
}

HRESULT CSteve::Ready_Mesh()
{
	Matrix temp = {};

	// �Ӹ�
	temp = MAtrixTranslation(0, 4.f / 16.f, 0.f) * vecBones[2].transform * vecBones[1].transform * vecBones[0].transform;
	m_pVIBufferCom[0]->SetMatrix(temp);

	// ����
	temp = MAtrixTranslation(0, 6.f / 16.f, 0.f) * vecBones[1].transform * vecBones[0].transform;
	m_pVIBufferCom[1]->SetMatrix(temp);

	// �ٸ�
	temp = MAtrixTranslation(0, -6.f / 16.f, 0.f) * vecBones[3].transform * vecBones[1].transform * vecBones[0].transform;
	m_pVIBufferCom[2]->SetMatrix(temp);

	temp = MAtrixTranslation(0, -6.f / 16.f, 0.f) * vecBones[4].transform * vecBones[1].transform * vecBones[0].transform;
	m_pVIBufferCom[3]->SetMatrix(temp);

	// ��
	temp = MAtrixTranslation(0, -6.f / 16.f, 0.f) * vecBones[5].transform * vecBones[1].transform * vecBones[0].transform;
	m_pVIBufferCom[4]->SetMatrix(temp);

	temp = MAtrixTranslation(0, -6.f / 16.f, 0.f) * vecBones[6].transform * vecBones[1].transform * vecBones[0].transform;
	m_pVIBufferCom[5]->SetMatrix(temp);

	return S_OK;
}


CSteve* CSteve::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSteve* pInstance = new CSteve(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSteve");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSteve::Clone(void* pArg)
{
	CSteve* pInstance = new CSteve(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSteve");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSteve::Free()
{
	__super::Free();

	Safe_Release(m_pRigidbodyCom);
	Safe_Release(m_pCollider_CubeCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

	for (int i = 0; i < 6; i++)
	{
		Safe_Release(m_pVIBufferCom[i]);
	}

}

