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
}

void CSteve::Update(_float fTimeDelta)
{
	if (GetKeyState(VK_UP) & 0x8000)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * -1.f);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta);
	}

	// 걷는 모션
	if (GetKeyState('Q') & 0x8000)
	{
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
}

void CSteve::Late_Update(_float fTimeDelta)
{
	// 본의 매트릭스를 현재 트랜스폼으로 업데이트
	vecBones[0].transform = *(m_pTransformCom->Get_WorldMatrix());

	// 본이 적용된 매쉬 업데이트
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

	return S_OK;
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

	// 머리
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Head"),
		TEXT("m_pVIBufferCom_Head"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[0]))))
		return E_FAIL;
	
	// 몸통
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Body"),
		TEXT("m_pVIBufferCom_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[1]))))
		return E_FAIL;

	// 다리
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Foot_R"),
		TEXT("m_pVIBufferCom_Foot_R"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[2]))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Steve_Foot_L"),
		TEXT("m_pVIBufferCom_Foot_L"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[3]))))
		return E_FAIL;

	// 팔
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

	// 머리
	temp = MAtrixTranslation(0, 4.f / 16.f, 0.f) * vecBones[2].transform * vecBones[1].transform * vecBones[0].transform;
	m_pVIBufferCom[0]->SetMatrix(temp);

	// 몸통
	temp = MAtrixTranslation(0, 6.f / 16.f, 0.f) * vecBones[1].transform * vecBones[0].transform;
	m_pVIBufferCom[1]->SetMatrix(temp);

	// 다리
	temp = MAtrixTranslation(0, -6.f / 16.f, 0.f) * vecBones[3].transform * vecBones[1].transform * vecBones[0].transform;
	m_pVIBufferCom[2]->SetMatrix(temp);

	temp = MAtrixTranslation(0, -6.f / 16.f, 0.f) * vecBones[4].transform * vecBones[1].transform * vecBones[0].transform;
	m_pVIBufferCom[3]->SetMatrix(temp);

	// 팔
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

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

	for (int i = 0; i < 6; i++)
	{
		Safe_Release(m_pVIBufferCom[i]);
	}

}

