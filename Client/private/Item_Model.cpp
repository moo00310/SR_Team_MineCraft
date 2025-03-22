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

    return S_OK;

}

void CItem_Model::Priority_Update(_float fTimeDelta)
{

}

void CItem_Model::Update(_float fTimeDelta)
{
    
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

HRESULT CItem_Model::Update_Anime(_int _type, _float fTimeDelta)
{
    fElapsedTime += fTimeDelta;

    if (fElapsedTime >= m_Animations[_type].back().fTime)
    {
        fElapsedTime = 0.0f;  // 처음부터 재생
    }

    if (m_Animations[_type].size() < 2)
    {
        // 단일 키프레임만 있을 경우 그냥 그거 하나 적용
        m_Bone.localTransform = m_Animations[_type][0].matTransform * m_Bone.baseTransform;
        return S_OK;
    }

    // 키프레임 찾기
    KEYFREAME key1, key2;
    for (size_t i = 0; i < m_Animations[_type].size() - 1; ++i)
    {
        if (fElapsedTime >= m_Animations[_type][i].fTime && fElapsedTime <= m_Animations[_type][i + 1].fTime)
        {
            key1 = m_Animations[_type][i];
            key2 = m_Animations[_type][i + 1];
            break;
        }
    }

    // 보간 비율 계산 (0~1 사이 값)
    float t = (fElapsedTime - key1.fTime) / (key2.fTime - key1.fTime);

    Matrix interpolatedMatrix = InterpolateMatrix_Quat(key1.matTransform, key2.matTransform, t);

    m_Bone.localTransform = interpolatedMatrix * m_Bone.baseTransform;

    return S_OK;
}

HRESULT CItem_Model::Update_BoneAndMesh(const const Matrix& matrix)
{
    m_Bone.worldTransform = m_Bone.localTransform * matrix;
    m_pTransformCom->Set_Matrix(m_Bone.Correction * m_Bone.worldTransform);
    return S_OK;
}

void CItem_Model::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
