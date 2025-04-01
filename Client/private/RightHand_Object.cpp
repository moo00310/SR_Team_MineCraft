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
    m_isActive = false;
    m_isRender = false;

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
    KeyInput();
}

void CRightHand_Object::Late_Update(_float fTimeDelta)
{
    Update_State(fTimeDelta);

    if (FAILED(Update_Root(fTimeDelta)))
        return;

    if (m_isRender)
    {
        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
            return;
    }
   
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

void CRightHand_Object::ChangeTexture(int Num)
{
    m_TextrueNum = Num;
}

HRESULT CRightHand_Object::Ready_Components()
{
    // 본 + 애니메이션
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
    case INIT:
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

void CRightHand_Object::KeyInput()
{
    if (m_pGameInstance->Key_Down(VK_LBUTTON))
    {
        m_eCurAnim = SWING;
        isAttack = true;
        return;
    }

    if (m_eCurAnim == SWING)
        return;

    // 애니메이션 바꾸기
    if (m_pGameInstance->Key_Pressing('W') ||
        m_pGameInstance->Key_Pressing('A') ||
        m_pGameInstance->Key_Pressing('S') ||
        m_pGameInstance->Key_Pressing('D'))
    {
        m_eCurAnim = WALK;
    }
    else
    {
        m_eCurAnim = INIT;
    }
}

void CRightHand_Object::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pSkeletalAnimator);

    for (auto& pVIBuffer : m_pVIBufferComs)
        Safe_Release(pVIBuffer);
}
