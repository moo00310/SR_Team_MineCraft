#include "Steve.h"

#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"
#include <iostream>

CSteve::CSteve(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CSteve::CSteve(const CSteve& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CSteve::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSteve::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	if (FAILED(Ready_Bone()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;
	
	return S_OK;
}

void CSteve::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_CollisionGroup(COLLISION_PLAYER, this);

	Input_Key(fTimeDelta);

}

void CSteve::Update(_float fTimeDelta)
{
	if (FAILED(m_pCollider_CubeCom->Update_ColliderBox()))
	{
		MSG_BOX("Update_ColliderBox()");
		return;
	}

	m_pRigidbodyCom->Update(fTimeDelta, COLLISION_BLOCK);

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

	if (m_pGameInstance->Key_Down(VK_F5))
	{
		m_bisTPS *= -1;
	}
	if (m_bisTPS > 0)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
			return;
	}

	m_skelAnime->Update_Bone(0, *m_pTransformCom->Get_WorldMatrix());

	// 애니메이션 적용
	if (m_AnimState == WALK)
	{
		m_skelAnime->Update_Animetion(WALK_F, fTimeDelta, 3, *m_pTransformCom->Get_WorldMatrix());
		m_skelAnime->Update_Animetion(WALK_B, fTimeDelta, 4, *m_pTransformCom->Get_WorldMatrix());
		m_skelAnime->Update_Animetion(WALK_F, fTimeDelta, 5, *m_pTransformCom->Get_WorldMatrix());
		m_skelAnime->Update_Animetion(WALK_B, fTimeDelta, 6, *m_pTransformCom->Get_WorldMatrix());
	}
	else if (m_AnimState == IDLE)
	{
		if (!m_skelAnime->IsBlending() && m_skelAnime->GetCurrentAnim() != ANIM::IDLE)
		{
			m_skelAnime->Start_Blend(WALK_F, IDLE, 0.005f);
			m_skelAnime->Start_Blend(WALK_B, IDLE, 9999999.f);
		}

		m_skelAnime->Update_Animetion(IDLE, fTimeDelta, 3, *m_pTransformCom->Get_WorldMatrix());
		m_skelAnime->Update_Animetion(IDLE, fTimeDelta, 4, *m_pTransformCom->Get_WorldMatrix());
		m_skelAnime->Update_Animetion(IDLE, fTimeDelta, 5, *m_pTransformCom->Get_WorldMatrix());
		m_skelAnime->Update_Animetion(IDLE, fTimeDelta, 6, *m_pTransformCom->Get_WorldMatrix());
	}
}
		

HRESULT CSteve::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	for (int i = 0; i < 6; i++)
	{
		if (FAILED(m_pVIBufferComs[i]->Bind_WorldMatrix()))
			return E_FAIL;

		if (FAILED(m_pVIBufferComs[i]->Bind_Buffers()))
			return E_FAIL;

		if (FAILED(m_pVIBufferComs[i]->Render()))
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

void CSteve::Input_Key(_float fTimeDelta)
{
	CURSORINFO tCursorInfo;
	tCursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&tCursorInfo);

	if (tCursorInfo.flags == CURSOR_SHOWING)
		return;

	Move(fTimeDelta);
	Turn(fTimeDelta);
}

void CSteve::Move(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Pressing('W'))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
		m_AnimState = WALK;
	}
	else
		m_AnimState = IDLE;

	if (m_pGameInstance->Key_Pressing('S'))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing('A'))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing('D'))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (m_pGameInstance->Key_Down(VK_SPACE))
	{
		if (m_pRigidbodyCom->Jump())
		{
			//m_pGameInstance->Play_Sound("event:/Built_Fail");
		}
		
	}
}

void CSteve::Turn(_float fTimeDelta)
{
	// 창이 활성화 상태가 아닐 경우 마우스 입력을 무시
	if (!(GetForegroundWindow() == g_hWnd))
		return;

	// 화면 중앙 좌표 계산
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	POINT ptCenter = { rc.right / 2, rc.bottom / 2 };

	// 현재 마우스 좌표 가져오기
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	// === 마우스가 창 내부에 있는지 확인 ===
	if (ptMouse.x < 0 || ptMouse.x >= rc.right || ptMouse.y < 0 || ptMouse.y >= rc.bottom)
		return;

	// 마우스 이동량 계산 (중앙 기준)
	_int iMouseMoveX = ptMouse.x - ptCenter.x;

	// 카메라의 yaw 값을 사용하여 스티브의 회전값을 설정
	m_pTransformCom->Turn({ 0.f, 1.f, 0.f }, fTimeDelta * iMouseMoveX * m_fMouseSensor);

	// 스티브의 월드 매트릭스를 가져옴
	const _float4x4* pWorldMatrix = m_pTransformCom->Get_WorldMatrix();

	// 'Look' 벡터를 얻기 위해, 월드 매트릭스에서 방향 벡터를 추출
	_float3 vLook = { pWorldMatrix->_31, pWorldMatrix->_32, pWorldMatrix->_33 };
	_float3 vUp = { pWorldMatrix->_21, pWorldMatrix->_22, pWorldMatrix->_23 };

	// 'Right' 벡터 계산: Look과 Up의 외적
	_float3 vRight;
	D3DXVec3Cross(&vRight, &vUp, &vLook);

	// 마우스 이동에 따른 회전값 적용
	_float fYaw = iMouseMoveX * fTimeDelta * m_fMouseSensor;  // 회전 각도 계산 (yaw)

	// 회전 행렬 생성 (Y축 기준 회전)
	_float4x4 matRotation;
	D3DXMatrixRotationAxis(&matRotation, &vUp, fYaw);  // Y축 기준으로 회전

	// 기존 월드 매트릭스를 회전 행렬로 갱신
	_float4x4 matNewWorld = *pWorldMatrix;
	matNewWorld = matRotation * matNewWorld;

	// 새로운 월드 매트릭스를 설정
	m_pTransformCom->Set_Matrix(matNewWorld);

	//// 마우스를 중앙으로 이동
	//ClientToScreen(g_hWnd, &ptCenter);
	//SetCursorPos(ptCenter.x, ptCenter.y);
}

HRESULT CSteve::Ready_Components()
{
	// 스티브 텍스처
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
	m_pVIBufferComs.resize(6);
	// 몸통
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Body"),
		TEXT("m_pVIBufferCom_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[0]))))
		return E_FAIL;

	// 머리
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Head"),
		TEXT("m_pVIBufferCom_Head"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[1]))))
		return E_FAIL;

	// 다리
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Foot_R"),
		TEXT("m_pVIBufferCom_Foot_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[2]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Foot_L"),
		TEXT("m_pVIBufferCom_Foot_L"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[3]))))
		return E_FAIL;

	// 팔
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Arm_R"),
		TEXT("m_pVIBufferCom_Arm_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[4]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Arm_L"),
		TEXT("m_pVIBufferCom_Arm_L"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[5]))))
		return E_FAIL;


	// 본 + 애니메이션
	CSkeletalAnimator::DESC DescSekel = { m_pVIBufferComs };
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SkeletalAnimator"),
		TEXT("m_pSkeletalAnimatorCom"), reinterpret_cast<CComponent**>(&m_skelAnime), &DescSekel)))
		return E_FAIL;


	//콜라이더
	/* For.Com_Collider */
	CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
	Desc.fRadiusX = 0.5f; Desc.fRadiusY = 1.f; Desc.fRadiusZ = 0.5;
	Desc.fOffSetY = 1.f;
	Desc.pTransformCom = m_pTransformCom;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
		TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pCollider_CubeCom), &Desc)))
		return E_FAIL;

	//리지드바디
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
		 { "Root"  , -1,  MAtrixTranslation(0.f, 0.f,0.f),	MAtrixTranslation(0.f	,0.f,	0.f),	Matrix(), Matrix() },  // root
		 { "Pelvis",  0,  MAtrixTranslation(0.f,  12.f / 16.f,0.f),	MAtrixTranslation(0.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, 6.f / 16.f, 0.f)},
		 { "Neck"  ,  1,  MAtrixTranslation(0.f,  24.f / 16.f,0.f),	MAtrixTranslation(0.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, 4.f / 16.f, 0.f)},
		 { "Leg_R",  1,	  MAtrixTranslation(2.f / 16.f,  0.f / 16.f,	0.f),	MAtrixTranslation(2.f / 16.f,	 0,		0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Leg_L" ,  1,  MAtrixTranslation(-2.f / 16.f,  0.f / 16.f,	0.f),	MAtrixTranslation(-2.f / 16.f,     0,	0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Arm_R" ,  1,  MAtrixTranslation(6.f / 16.f,  12.f / 16.f,	0.f),	MAtrixTranslation(6.f / 16.f,   12.f / 16.f	,0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
		 { "Arm_L" ,  1,  MAtrixTranslation(-6.f / 16.f,  12.f / 16.f,	0.f),	MAtrixTranslation(-6.f / 16,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
	};

	for (int i = 0; i < 7; i++)
	{
		m_skelAnime->Add_Bone(bone[i]);
	}

	return S_OK;
}

HRESULT CSteve::Ready_Animation()
{
	/*----------
	* 위치 초기화
	------------*/

	Matrix mat99 = {};
	mat99.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(0));

	KEYFREAME IDLE = { 1.f, mat99 };
	m_skelAnime->Add_Animation(ANIM::IDLE, IDLE);

	/*----------
	* Walk 모션
	------------*/
	Matrix mat = {};
	mat.Turn_Radian(_float3(1.f,0.f, 0.f), D3DXToRadian(0));

	Matrix mat2 = {};
	mat2.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(60));

	Matrix mat3 = {};
	mat3.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(0));

	Matrix mat4 = {};
	mat3.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-60));
	
	KEYFREAME Walk_1_F = { 0.f, mat };
	KEYFREAME Walk_2_F = { 1.0f, mat2 };
	KEYFREAME Walk_3_F = { 2.0f, mat3 };
	KEYFREAME Walk_4_F = { 3.0f, mat4 };

	KEYFREAME Walk_1_B = { 0.f,  mat4 };
	KEYFREAME Walk_2_B = { 1.0f, mat3 };
	KEYFREAME Walk_3_B = { 2.0f, mat2 };
	KEYFREAME Walk_4_B = { 3.0f, mat };


	m_skelAnime->Add_Animation(ANIM::WALK_F, Walk_1_F);
	m_skelAnime->Add_Animation(ANIM::WALK_F, Walk_2_F);
	m_skelAnime->Add_Animation(ANIM::WALK_F, Walk_3_F);
	m_skelAnime->Add_Animation(ANIM::WALK_F, Walk_4_F);

	m_skelAnime->Add_Animation(ANIM::WALK_B, Walk_1_B);
	m_skelAnime->Add_Animation(ANIM::WALK_B, Walk_2_B);
	m_skelAnime->Add_Animation(ANIM::WALK_B, Walk_3_B);
	m_skelAnime->Add_Animation(ANIM::WALK_B, Walk_4_B);

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
	Safe_Release(m_skelAnime);
	for (auto& buffer : m_pVIBufferComs)
		Safe_Release(buffer);

}

