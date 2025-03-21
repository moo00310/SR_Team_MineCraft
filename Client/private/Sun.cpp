#include "Sun.h"

CSun::CSun(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CSun::CSun(const CSun& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CSun::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSun::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSun::Priority_Update(_float fTimeDelta)
{
}

void CSun::Update(_float fTimeDelta)
{
	
}


void CSun::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);

	Orbit_Around_Earth();
}

HRESULT CSun::Render()
{
	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	SetUp_RenderState();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	Release_RenderState();
}

void CSun::Orbit_Around_Earth()
{
	// �� ����� �����ͼ� ī�޶� ��ġ�� �˾Ƴ���.
	_float4x4 ViewMatrix{};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	// View ����� ������� ���Ͽ� ���� ��ǥ�迡�� ī�޶� ��ġ�� ����
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	// ī�޶� ��ġ (ViewMatrix�� ������ ��)
	_float3 vCamPos{ *reinterpret_cast<_float3*>(&ViewMatrix.m[3][0]) };

	// vOriginPos: ȸ���� ����� �ʱ� ��ġ (ī�޶� �������� ���� z+10 ��ġ)
	_float3 vOriginPos{ 0.f, 0.f, 1.f };

	// �¾��� �ʱ� ��ġ (ī�޶� �������� ���� �Ÿ�, z+10)
	_float3 vSunPos = vCamPos + vOriginPos;

	// ȸ�� ��ȯ ���� (X�� ���� ȸ��)
	static float fAngle = 0.f;
	fAngle += 0.01f; // ȸ�� �ӵ� ����

	_float4x4 matRotX;
	D3DXMatrixRotationX(&matRotX, fAngle); // X�� ȸ�� ��� ����

	// vOriginPos�� �������� ȸ�� ��ȯ ����
	D3DXVec3TransformCoord(&vSunPos, &vOriginPos, &matRotX);

	// ȸ���� ��ġ�� �ٽ� ī�޶� ��ġ�� �������� ��ȯ
	vSunPos += vCamPos;

	// �¾� ��ġ ����
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vSunPos);

	// �¾��� �׻� ī�޶� �ٶ󺸵��� ���� (Billboard ȿ��)
	m_pTransformCom->LookAt(vCamPos);
}

HRESULT CSun::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 1);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	return S_OK;
}

HRESULT CSun::Release_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CSun::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_Texture_Sun"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

CSun* CSun::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSun* pInstance = new CSun(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSun::Clone(void* pArg)
{
	CSun* pInstance = new CSun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSun::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
