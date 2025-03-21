#include "Item_Model.h"

CItem_Model::CItem_Model(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CItem_Model::CItem_Model(const CItem_Model& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CItem_Model::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_Model::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;


    // 스윙 프레임 행렬을 벡터에 저장
    Matrix mat = {};
    mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(70));
    mat.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(45));
    mat.Set_State(mat.STATE_POSITION, _float3(1.f, -0.5f, 1.2f));

    Matrix matrix = {};
    matrix.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(100));
    matrix.Set_State(matrix.STATE_POSITION, _float3(-1.5f, 0.2f, -0.3f));


    m_swing.push_back({ 0.f, mat });
    m_swing.push_back({ 0.3f, matrix * mat });

    return S_OK;

}

void CItem_Model::Priority_Update(_float fTimeDelta)
{

}

void CItem_Model::Update(_float fTimeDelta)
{
    Matrix		ViewMatrix = {};
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);
   

    // 버튼 클릭 애니메이션
    if (GetKeyState(VK_LBUTTON) & 0x8000)
    {
        fElapsedTime += fTimeDelta;
    }
    else
    {
        // 스윙하지 않으면 처음 위치를 따라다님
        m_pTransformCom->Set_Matrix(m_swing[0].matTransform * ViewMatrix);
    }  

    if (fElapsedTime >= m_swing.back().fTime)
    {
        fElapsedTime = 0.0f;  // 처음부터 재생
    }

    // 키프레임 찾기
    KEYFREAME key1, key2;
    for (size_t i = 0; i < m_swing.size() - 1; ++i)
    {
        if (fElapsedTime >= m_swing[i].fTime && fElapsedTime <= m_swing[i + 1].fTime)
        {
            key1 = m_swing[i];
            key2 = m_swing[i + 1];
            break;
        }
    }

     // 보간 비율 계산 (0~1 사이 값)
    float t = (fElapsedTime - key1.fTime) / (key2.fTime - key1.fTime);

    // 행렬 보간
    D3DXMATRIX interpolatedMatrix = InterpolateMatrix(key1.matTransform, key2.matTransform, t);

    // 최종 행렬 적용
    m_pTransformCom->Set_Matrix(interpolatedMatrix * ViewMatrix);
}

void CItem_Model::Late_Update(_float fTimeDelta)
{
}

HRESULT CItem_Model::Render()
{
    if (FAILED(SetUp_RenderState()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;
    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    if (FAILED(Release_RenderState()))
        return E_FAIL;
    
	return S_OK;
}

HRESULT CItem_Model::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(m_TextureLevel, m_TextureTag,
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect_Normal"),
        TEXT("m_pVIBufferCom"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

	return S_OK;
}

HRESULT CItem_Model::SetUp_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 254);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    return S_OK;
}

HRESULT CItem_Model::Release_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    return S_OK;
}

CItem_Model* CItem_Model::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	return nullptr;
}

CGameObject* CItem_Model::Clone(void* pArg)
{
	return nullptr;
}

void CItem_Model::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
