#include "Arm_Steve.h"
#include "GameInstance.h"

CArm_Steve::CArm_Steve(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CArm_Steve::CArm_Steve(const CArm_Steve& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CArm_Steve::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CArm_Steve::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Bone()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	return S_OK;
}

void CArm_Steve::Priority_Update(_float fTimeDelta)
{
}

void CArm_Steve::Update(_float fTimeDelta)
{
	KeyInput();
}

void CArm_Steve::Late_Update(_float fTimeDelta)
{
	// �ִϸ��̼� ü���� �ݿ�
	Update_State(fTimeDelta);

	// ��Ʈ���� ���󰡴� ��
	if (FAILED(Update_Root(fTimeDelta)))
		return;

	// ī�޶� ���������� ������ �ø��� �ݿ�
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

HRESULT CArm_Steve::Render()
{ 
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferComs[0]->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pVIBufferComs[0]->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferComs[0]->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CArm_Steve::Ready_Components()
{
	// ��Ƽ�� �ؽ�ó
  /* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Steve"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	m_pVIBufferComs.resize(1);

	// ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Arm_R"),
		TEXT("m_pVIBufferCom_Arm_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[0]))))
		return E_FAIL;

	// �� + �ִϸ��̼�
	CSkeletalAnimator::DESC DescSekel = { m_pVIBufferComs};
	int a = 10;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SkeletalAnimator"),
		TEXT("m_pSkeletalAnimatorCom"), reinterpret_cast<CComponent**>(&m_pSkeletalAnimator), &DescSekel)))
		return E_FAIL;

	return S_OK;
}

HRESULT CArm_Steve::Ready_Bone()
{
	Matrix mat = {};
	mat.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-115));
	mat.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-20));
	mat.Set_State(mat.STATE_POSITION, _float3(0.8f, -0.5f, 0.8f));

	BONE bone = { "root", -1, mat, mat, Matrix(), Matrix() };

	m_pSkeletalAnimator->Add_Bone(bone);

	return S_OK;
}

HRESULT CArm_Steve::Ready_Animation()
{
	/*-------------------
	 �� ���� ���
	--------------------*/
	Matrix matrix = {};
	matrix.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-30));
	matrix.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(-15));
	matrix.Set_State(matrix.STATE_POSITION, _float3(0.f, -0.5f, -0.3f));

	Matrix matrix2 = { matrix };
	matrix2.Set_State(matrix2.STATE_POSITION, _float3(0.1f, 0.5f, -0.3f));

	KEYFREAME keyframe = { 0.f, Matrix() };
	KEYFREAME keyframe2 = { 0.1f, matrix };
	KEYFREAME keyframe3 = { 0.3f, matrix2 };

	m_pSkeletalAnimator->Add_Animation(SWING, keyframe);
	m_pSkeletalAnimator->Add_Animation(SWING, keyframe2);
	m_pSkeletalAnimator->Add_Animation(SWING, keyframe3);


	/*-------------------
	  �� IDLE ���
	--------------------*/
	keyframe = { 0.f, Matrix() };
	m_pSkeletalAnimator->Add_Animation(IDLE, keyframe);

	/*-------------------
	 �� WALK ���
	--------------------*/

	matrix = {};
	matrix.Set_State(matrix.STATE_POSITION, _float3(-0.01f, -0.08f, -0.01f));

	matrix2 = {};
	matrix2.Set_State(matrix.STATE_POSITION, _float3(-0.08f, 0.01f, -0.08f));

	keyframe =  { 0.f, Matrix() };
	keyframe2 = { 0.4f, matrix };
	keyframe3 = { 0.6f, matrix2 };
	KEYFREAME keyframe4 = { 0.8f,	matrix };
	KEYFREAME keyframe5 = { 1.f, Matrix()};

	m_pSkeletalAnimator->Add_Animation(WALK, keyframe);
	m_pSkeletalAnimator->Add_Animation(WALK, keyframe2);
	m_pSkeletalAnimator->Add_Animation(WALK, keyframe3);
	m_pSkeletalAnimator->Add_Animation(WALK, keyframe4);
	m_pSkeletalAnimator->Add_Animation(WALK, keyframe5);

	return S_OK;
}

HRESULT CArm_Steve::Update_Root(_float fTimeDelta)
{
	Matrix		ViewMatrix = {};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	// ī�޶� ��ġ�� ������� �ϴ� �ڵ���
	m_pSkeletalAnimator->Update_RootBone(ViewMatrix);

	return S_OK;
}

void CArm_Steve::Update_State(_float fTimeDelta)
{
	switch (m_eCurAnim)
	{
	case CArm_Steve::IDLE:
		Motion_Idle(fTimeDelta);
		break;
	case CArm_Steve::SWING:
		Motion_Swing(fTimeDelta);
		break;
	case CArm_Steve::WALK:
		Motion_Walk(fTimeDelta);
		break;
	case CArm_Steve::ANIM_END:
		break;
	default:
		break;
	}
}

void CArm_Steve::Motion_Idle(_float fTimeDelta)
{
	m_pSkeletalAnimator->Update_Animetion(IDLE, fTimeDelta, 0);

	if (m_pSkeletalAnimator->is_AnimtionEND(IDLE))
	{
		m_eCurAnim = IDLE;
	}
}

void CArm_Steve::Motion_Swing(_float fTimeDelta)
{
	m_pSkeletalAnimator->Update_Animetion(SWING, fTimeDelta, 0);

	if (m_pSkeletalAnimator->is_AnimtionEND(SWING))
	{
		m_eCurAnim = IDLE;
		isAttack = false;
	}
}

void CArm_Steve::Motion_Walk(_float fTimeDelta)
{
	m_pSkeletalAnimator->Update_Animetion(WALK, fTimeDelta, 0);

	if (m_pSkeletalAnimator->is_AnimtionEND(WALK))
	{
		m_eCurAnim = WALK;
	}
}

void CArm_Steve::KeyInput()
{
	if (m_pGameInstance->Key_Down(VK_LBUTTON))
	{
		m_eCurAnim = SWING;
		isAttack = true;
		return;
	}

	if (m_eCurAnim == SWING)
		return;
	
	// �ִϸ��̼� �ٲٱ�
	if (m_pGameInstance->Key_Pressing('W') ||
		m_pGameInstance->Key_Pressing('A') ||
		m_pGameInstance->Key_Pressing('S') ||
		m_pGameInstance->Key_Pressing('D'))
	{
		m_eCurAnim = WALK;
	}
	else
	{
		m_eCurAnim = IDLE;
	}
}

CArm_Steve* CArm_Steve::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CArm_Steve* pInstance = new CArm_Steve(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CArm_Steve");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CArm_Steve::Clone(void* pArg)
{
	CArm_Steve* pInstance = new CArm_Steve(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CArm_Steve");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArm_Steve::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pSkeletalAnimator);

	for (auto& pVIBuffer : m_pVIBufferComs)
		Safe_Release(pVIBuffer);

}
