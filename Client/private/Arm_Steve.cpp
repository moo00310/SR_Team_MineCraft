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
	
	return S_OK;
}

void CArm_Steve::Priority_Update(_float fTimeDelta)
{
}

void CArm_Steve::Update(_float fTimeDelta)
{
	Matrix		ViewMatrix = {};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
	Matrix mat = {};
	mat.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-115));
	mat.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-20));
	mat.Set_State(mat.STATE_POSITION, _float3(0.4f, -0.4f, 0.6f));

	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		Matrix matrix = {};
		matrix.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-30));
		matrix.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(-15));
		matrix.Go_Vector(matrix.STATE_UP, fTimeDelta, -25.f);
		matrix.Go_Straight(fTimeDelta, -15.f);
		m_pTransformCom->Set_Matrix(matrix * mat * ViewMatrix);
	}
	else
	{
		m_pTransformCom->Set_Matrix(mat * ViewMatrix);
	}
}

void CArm_Steve::Late_Update(_float fTimeDelta)
{
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

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CArm_Steve::Ready_Components()
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

	// 팔
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Arm_R"),
		TEXT("m_pVIBufferCom_Arm_R"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
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

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}
