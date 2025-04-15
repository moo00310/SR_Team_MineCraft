#include "BossUI.h"

CBossUI::CBossUI(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject{ pGraphic_Device }
{
}

CBossUI::CBossUI(CBossUI& Prototype)
	:CGameObject( Prototype )
{
}

HRESULT CBossUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossUI::Initialize(void* pArg)
{
	m_pPawn = dynamic_cast<CPawn*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Monster")));
	Safe_AddRef(m_pPawn);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBossUI::Priority_Update(_float fTimeDelta)
{
}

void CBossUI::Update(_float fTimeDelta)
{
	if (!m_pPawn->GetActive() || m_pPawn == nullptr) return;
	
	_float3 vPos = m_pPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION) + _float3(0.f, 3.f, 0.f);
	Compout_Billboard(vPos, 3, 0.3f);
}

void CBossUI::Late_Update(_float fTimeDelta)
{
	if (!m_pPawn->GetActive()) return;

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this)))
		return;
}

HRESULT CBossUI::Render()
{
	if (!m_pPawn->GetActive()) return S_OK;


	if (FAILED(Render_UI()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossUI::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_BossUi"), TEXT("Com_Texture"),
		reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_BossUI_Back"), TEXT("Com_Texture1"),
		reinterpret_cast<CComponent**>(&m_pTextureCom_Back))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"),
		reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Billboard_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CBossUI::Compout_Billboard(_float3& vPos, float fWidth, float fHeight)
{
	// 1. 뷰 행렬 가져오기
	D3DXMATRIX matView;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &matView);

	// 2. 카메라 방향 축 추출
	D3DXVECTOR3 vRight(matView._11, matView._21, matView._31); // 카메라 Right
	D3DXVECTOR3 vUp(0.f, 1.f, 0.f);                             // UI는 항상 위를 고정 (Y-up)
	D3DXVECTOR3 vLook(matView._13, matView._23, matView._33);  // 카메라 Look

	// 3. 축 정규화 및 크기 조절
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	vRight *= fWidth;
	vUp *= fHeight;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, _float3(vRight.x, vRight.y, vRight.z));
	m_pTransformCom->Set_State(CTransform::STATE_UP, _float3(vUp.x, vUp.y, vUp.z));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, _float3(vLook.x, vLook.y, vLook.z));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(vPos.x, vPos.y, vPos.z));
}

HRESULT CBossUI::Render_UI()
{
	D3DXMATRIX viewMatrix, projMatrix;
	if (FAILED(m_pGraphic_Device->GetTransform(D3DTS_VIEW, &viewMatrix)))
		return E_FAIL;
	if (FAILED(m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &projMatrix)))
		return E_FAIL;


	if (FAILED(m_pTextureCom_Back->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &viewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &projMatrix)))
		return E_FAIL;

	// 배경
	m_pShaderCom->Bind_Texture("g_Texture", m_pTextureCom_Back->Get_Texture(0));
	m_pShaderCom->Begin(0);
	m_pShaderCom->SetFloat("g_Bright", g_fBright + 0.4f);

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();

	////// HP
	m_pShaderCom->Bind_Texture("g_Texture", m_pTextureCom->Get_Texture(0));
	m_pShaderCom->Begin(1);

	float ratio = m_pPawn->Get_Hp() / 500.f;

	m_pShaderCom->SetFloat("g_Bright", g_fBright + 0.4f);
	m_pShaderCom->SetFloat("g_BossHp", ratio);

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pShaderCom->End();

	return S_OK;
}


CBossUI* CBossUI::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBossUI* pInstance = new CBossUI(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBossUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBossUI::Clone(void* pArg)
{
	CBossUI* pInstance = new CBossUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBossUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBossUI::Free()
{
	__super::Free();

	Safe_Release(m_pPawn);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_Back);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
