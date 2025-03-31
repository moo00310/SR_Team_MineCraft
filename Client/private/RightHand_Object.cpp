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
    Update_State(fTimeDelta);
}

HRESULT CRightHand_Object::Render()
{

    if (FAILED(m_pVIBufferComs[0]->Bind_WorldMatrix()))
        return E_FAIL;

    if (FAILED(m_pVIBufferComs[0]->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferComs[0]->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CRightHand_Object::Ready_Components()
{
    // 본 + 애니메이션
    m_pVIBufferComs.resize(1);
    CSkeletalAnimator::DESC DescSekel = { m_pVIBufferComs };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SkeletalAnimator"),
        TEXT("m_pSkeletalAnimatorCom"), reinterpret_cast<CComponent**>(&m_pSkeletalAnimator), &DescSekel)))
        return E_FAIL;

    return S_OK;
}


void CRightHand_Object::Update_State(_float fTimeDelta)
{
    switch (m_eCurAnim)
    {
    case IDLE:
        Motion_Idle(fTimeDelta);
        break;
    case SWING:
        Motion_Swing(fTimeDelta);
        break;
    case WALK:
        Motion_Walk(fTimeDelta);
        break;
    case ANIM_END:
        break;
    default:
        break;
    }
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
}
