#include "Sun.h"
#include "BreakableCube.h"
#include "BreakableRect.h"
#include "Tree.h"
#include "SkyBox.h"
#include "Pawn.h"
#include "RightHand_Object.h"
#include "Clouds.h"

_float g_fBright = 0.75f;

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

	m_pTransformCom->Scaling(0.4f, 0.4f, 0.4f);
	return S_OK;
}

void CSun::Priority_Update(_float fTimeDelta)
{
	m_brightFrame++;

	if (m_brightFrame > 20) {
		float t = (m_fAngle + 60.5f) / (-56.f + 60.5f); // 0 ~ 1 변환
		float bright = 0.5f + 0.5f * sinf(t * D3DX_PI); // sin(0) = 0, sin(PI) = 1

		if (m_isSun) {
			g_fBright = bright;
		}
		else {
			g_fBright = 1 - bright;
		}
		
		m_brightFrame = 0;

		if (g_fBright <= 0.1f || g_fBright >= 1.f) {
			m_fBrightPercent *= -1;
		}
	}
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
	if (m_isSun) {
		if (FAILED(m_pTextureCom->Bind_Resource(0)))
			return E_FAIL;
	}
	else {
		if (FAILED(m_pTextureCom2->Bind_Resource(0)))
			return E_FAIL;
	}


	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	SetUp_RenderState();

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	Release_RenderState();

	return S_OK;
}

float CSun::GetBight() const
{
	return g_fBright;
}

void CSun::Orbit_Around_Earth()
{
	// 뷰 행렬을 가져와서 카메라 위치를 알아낸다.
	_float4x4 ViewMatrix{};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	// View 행렬의 역행렬을 구하여 월드 좌표계에서 카메라 위치를 추출
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	// 카메라 위치 (ViewMatrix의 마지막 행)
	_float3 vCamPos{ *reinterpret_cast<_float3*>(&ViewMatrix.m[3][0]) };

	// vOriginPos: 회전할 대상의 초기 위치 (카메라 기준으로 앞쪽 z+10 위치)
	_float3 vOriginPos{ 0.f, 0.f, 1.f };

	// 태양의 초기 위치 (카메라 앞쪽으로 일정 거리, z+10)
	_float3 vSunPos = vCamPos + vOriginPos;

	// 회전 변환 적용 (X축 기준 회전)
	if (m_bAddTimeFast) {
		m_fAngle += 0.015f; // 회전 속도 증가
	}
	

	if (-56 < m_fAngle) {
		m_isSun = !m_isSun;
		m_fAngle = -60.5f;
	}

	_float4x4 matRotX;
	D3DXMatrixRotationX(&matRotX, m_fAngle); // X축 회전 행렬 생성

	// vOriginPos를 기준으로 회전 변환 적용
	D3DXVec3TransformCoord(&vSunPos, &vOriginPos, &matRotX);

	// 회전된 위치를 다시 카메라 위치를 기준으로 변환
	vSunPos += vCamPos;

	// 태양 위치 설정
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vSunPos);

	// 태양이 항상 카메라를 바라보도록 설정 (Billboard 효과)
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
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Sun"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Moon"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom2))))
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
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
