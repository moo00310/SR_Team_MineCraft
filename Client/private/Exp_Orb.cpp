#include "Exp_Orb.h"
#include "MCTerrain.h"
#include "Steve.h"

CExp_Orb::CExp_Orb(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CExp_Orb::CExp_Orb(const CExp_Orb& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CExp_Orb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CExp_Orb::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 30.f, 0.f));
	m_pTransformCom->Scaling(0.01f, 0.01f, 0.01f);
	//m_pTransformCom->Rotation({ 1.f, 0.f, 0.f }, D3DXToRadian(-90.f));

	const _float fPower{ 1.5f };
	const _float fHorizonPower{ (rand() % 11 - 5) * fPower };
	const _float fVerticalPower{ (rand() % 11 + 3) * fPower };
	m_pRigidbodyCom->Knock_back(_float3{ fHorizonPower,fVerticalPower , fHorizonPower });

	return S_OK;
}

void CExp_Orb::Priority_Update(_float fTimeDelta)
{
}

void CExp_Orb::Update(_float fTimeDelta)
{

	CSteve* pSteve = static_cast<CSteve*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve")));

	_float3 vStevePos = pSteve->Get_Transform()->Get_State(CTransform::STATE_POSITION);

	_float3 vEyePos = { vStevePos + _float3{0.f, 1.5f, 0.f} };


	_float3 vDiff{ vEyePos - m_pTransformCom->Get_State(CTransform::STATE_POSITION) };

	_float fDist = D3DXVec3Length(&vDiff);

	if (fDist < 0.2f)
	{
		m_pGameInstance->Play_Sound(TEXT("orb"), SOUND_ITEM, this, 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

		Destroy();
	}
	else if (fDist < 5.f && m_fTime > 2.f)
	{
		m_pTransformCom->Chase(vEyePos, fTimeDelta / fDist * 0.5f, 0.f);
	}
	else
	{
		CMCTerrain* pTerrain = static_cast<CMCTerrain*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Terrain")));

		list<CCollider*> Colliders;
		Colliders = pTerrain->Active_Near_Chunk_Colliders(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f);

		//if (!m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.f))
		//	return;

		//플레이어와 거리가 가까우면 중력적용
		m_pRigidbodyCom->Update_RayCast_InstancingObject(fTimeDelta, COLLISION_BLOCK, 0.3f);

		for (CCollider* pCollider : Colliders)
		{
			m_pGameInstance->Out_Collider_CollisiomGroup(COLLISION_BLOCK, pCollider);
		}
	}


	m_fTime += fTimeDelta;

	Look_At_Camera();
}


void CExp_Orb::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CExp_Orb::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(static_cast<_uint>(m_fTime * 10) % 12))) //12장 0~11
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	m_pTransformCom->Bind_Resource(m_pShaderCom);
	m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);
	m_pShaderCom->SetFloat("g_Bright", g_fBright + 0.2f);
	m_pShaderCom->Begin(0);

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();

	return S_OK;
}

void CExp_Orb::Look_At_Camera()
{
	// 뷰 행렬을 가져와서 카메라 위치를 알아낸다.
	_float4x4 ViewMatrix{};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	// View 행렬의 역행렬을 구하여 월드 좌표계에서 카메라 위치를 추출
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	// 카메라 위치 (ViewMatrix의 마지막 행)
	_float3 vCamPos{ *reinterpret_cast<_float3*>(&ViewMatrix.m[3][0]) };

	// 태양이 항상 카메라를 바라보도록 설정 (Billboard 효과)
	m_pTransformCom->LookAt(vCamPos);
}

HRESULT CExp_Orb::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Exp_Orb"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_RectShader"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Rigidbody */
	CRigidbody::RIGIDBODY_DESC RigidbodyDesc{};
	RigidbodyDesc.pTransform = m_pTransformCom;

	RigidbodyDesc.fMass = 0.1f;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Rigidbody"),
		TEXT("Com_Rigidbody"), reinterpret_cast<CComponent**>(&m_pRigidbodyCom), &RigidbodyDesc)))
		return E_FAIL;

	return S_OK;
}

CExp_Orb* CExp_Orb::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CExp_Orb* pInstance = new CExp_Orb(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CExp_Orb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CExp_Orb::Clone(void* pArg)
{
	CExp_Orb* pInstance = new CExp_Orb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CExp_Orb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExp_Orb::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRigidbodyCom);
}
