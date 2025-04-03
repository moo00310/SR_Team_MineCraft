#include "RightHand_Object.h"


CRightHand_Object::CRightHand_Object(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CRightHand_Object::CRightHand_Object(const CRightHand_Object& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CRightHand_Object::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRightHand_Object::Initialize(void* pArg)
{
    m_isRender = false;
    m_pSteve = static_cast<CSteve*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve")));

    if (m_pSteve == nullptr) return E_FAIL;
    Safe_AddRef(m_pSteve);

    m_pVIBufferComs.resize(1);
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Bone()))
        return E_FAIL;

    if (FAILED(Ready_Animation()))
        return E_FAIL;

    return S_OK;
}

void CRightHand_Object::Priority_Update(_float fTimeDelta)
{
}

void CRightHand_Object::Update(_float fTimeDelta)
{

}

void CRightHand_Object::Late_Update(_float fTimeDelta)
{
  
    if (!m_isTPS)
    {
        Update_State(fTimeDelta);

        if (FAILED(Update_Root(fTimeDelta)))
            return;

    }

    if (m_isRender)
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }
   
}

HRESULT CRightHand_Object::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(m_TextrueNum)))
        return E_FAIL;

    if (FAILED(m_pVIBufferComs[0]->Bind_WorldMatrix(m_pShaderCom)))
        return E_FAIL;

    if (FAILED(m_pVIBufferComs[0]->Bind_Buffers()))
        return E_FAIL;

    m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", m_TextrueNum);
    m_pShaderCom->SetFloat("g_Bright", g_fBright + 0.2f);
    m_pShaderCom->Begin(m_RederID);

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferComs[0]->Render()))
        return E_FAIL;

    m_pShaderCom->End();

    return S_OK;
}

void CRightHand_Object::ChangeTexture(int Num)
{
    m_TextrueNum = Num;
}

HRESULT CRightHand_Object::Ready_Components()
{
    // 쉐이더 컴포넌트
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Cube"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    // 본 + 애니메이션
    CSkeletalAnimator::DESC DescSekel = { m_pVIBufferComs };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SkeletalAnimator"),
        TEXT("m_pSkeletalAnimatorCom"), reinterpret_cast<CComponent**>(&m_pSkeletalAnimator), &DescSekel)))
        return E_FAIL;

    return S_OK;
}


HRESULT CRightHand_Object::Update_Root(_float fTimeDelta)
{
    Matrix		ViewMatrix = {};
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
    D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

    // 카메라 위치로 따라오게 하는 코드임
    m_pSkeletalAnimator->Update_RootBone(ViewMatrix);

    return S_OK;
}


void CRightHand_Object::Free()
{
    __super::Free();

    Safe_Release(m_pSteve);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pSkeletalAnimator);
    Safe_Release(m_pShaderCom);

    for (auto& pVIBuffer : m_pVIBufferComs)
        Safe_Release(pVIBuffer);
}
