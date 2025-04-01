#include "Cube_Model.h"

CCube_Model::CCube_Model(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CRightHand_Object{ pGraphic_Device }
{
}

CCube_Model::CCube_Model(const CCube_Model& Prototype)
    : CRightHand_Object( Prototype )
{
}

HRESULT CCube_Model::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCube_Model::Initialize(void* pArg)
{
    m_RederID = 1;
    m_TextrueNum = 0;

    __super::Initialize(pArg);
    return S_OK;
}

void CCube_Model::Priority_Update(_float fTimeDelta)
{
    KeyInput();
}

void CCube_Model::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CCube_Model::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

}

HRESULT CCube_Model::Render()
{

    __super::Render();

    return S_OK;
}

HRESULT CCube_Model::Ready_Components()
{
    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Cube_Model"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Anim_Cube"),
        TEXT("m_pVIBufferCom"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[0]))))
        return E_FAIL;

    __super::Ready_Components();

    return S_OK;
}

HRESULT CCube_Model::Ready_Bone()
{
    // 스윙 프레임 행렬을 벡터에 저장
    Matrix mat = {};
    mat.Scaling(0.5f, 0.5f, 0.5f);
    mat.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(80));
    mat.Turn_Radian(_float3(0.f, 1.f, 0.f), D3DXToRadian(45));
    mat.Set_State(mat.STATE_POSITION, _float3(1.f, -0.65f, 1.2f));

    BONE bone = { "root", -1, mat, mat, Matrix(), Matrix() };

    m_pSkeletalAnimator->Add_Bone(bone);

    return S_OK;
}

HRESULT CCube_Model::Ready_Animation()
{
    ///*------------------------
     //* Init 
     //----------------------------*/
    Matrix mat = {};
    KEYFREAME Init1 = { 0.f, mat };
    m_pSkeletalAnimator->Add_Animation(INIT, Init1);

    ///*------------------------
    //* Swing 애니메이션 
    //----------------------------*/
    Matrix matrix1 = {};
    matrix1.Turn_Radian(_float3(1.f, 1.f, 0.f), D3DXToRadian(110));
    matrix1.Set_State(matrix1.STATE_POSITION, _float3(-1.f, 1.f, 1.f));


    KEYFREAME Swing1 = { 0.f, mat };
    KEYFREAME Swing2 = { 0.15f, matrix1 };
    KEYFREAME Swing4 = { 0.3f, mat };

    m_pSkeletalAnimator->Add_Animation(SWING, Swing1);
    m_pSkeletalAnimator->Add_Animation(SWING, Swing2);
    m_pSkeletalAnimator->Add_Animation(SWING, Swing4);

    ///*------------------------
    //* WALK 애니메이션 
    //----------------------------*/

    matrix1 = {};
    matrix1.Set_State(matrix1.STATE_POSITION, _float3(-0.04f, -0.04f, -0.04f));

    Matrix matrix2 = {};
    matrix2.Set_State(matrix2.STATE_POSITION, _float3(-0.04f, 0.04f, -0.04f));

    KEYFREAME keyframe = { 0.f, mat };
    KEYFREAME keyframe2 = { 0.4f, matrix1 };
    KEYFREAME keyframe3 = { 0.6f, matrix2 };
    KEYFREAME keyframe4 = { 0.8f, matrix1 };
    KEYFREAME keyframe5 = { 1.f, mat };

    m_pSkeletalAnimator->Add_Animation(WALK, keyframe);
    m_pSkeletalAnimator->Add_Animation(WALK, keyframe2);
    m_pSkeletalAnimator->Add_Animation(WALK, keyframe3);
    m_pSkeletalAnimator->Add_Animation(WALK, keyframe4);
    m_pSkeletalAnimator->Add_Animation(WALK, keyframe5);


    return S_OK;
}

void CCube_Model::Motion_Idle(_float fTimeDelta)
{
    m_pSkeletalAnimator->Update_Animetion(INIT, fTimeDelta, 0);

    if (m_pSkeletalAnimator->is_AnimtionEND(INIT))
    {
        m_eCurAnim = INIT;
    }
}

void CCube_Model::Motion_Swing(_float fTimeDelta)
{
    m_pSkeletalAnimator->Update_Animetion(SWING, fTimeDelta, 0);

    if (m_pSkeletalAnimator->is_AnimtionEND(SWING))
    {
        m_eCurAnim = INIT;
    }
}

void CCube_Model::Motion_Walk(_float fTimeDelta)
{
    m_pSkeletalAnimator->Update_Animetion(WALK, fTimeDelta, 0);

    if (m_pSkeletalAnimator->is_AnimtionEND(WALK))
    {
        m_eCurAnim = WALK;
    }
}


CCube_Model* CCube_Model::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCube_Model* pInstance = new CCube_Model(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCube_Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCube_Model::Clone(void* pArg)
{
    CCube_Model* pInstance = new CCube_Model(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CCube_Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCube_Model::Free()
{
    __super::Free();
}

void CCube_Model::Update_State(_float fTimeDelta)
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
    case EAT:
    case ANIM_END:
        break;
    default:
        break;
    }
}

void CCube_Model::KeyInput()
{
    if (m_pGameInstance->Key_Down(VK_LBUTTON) ||
        m_pGameInstance->Key_Down(VK_RBUTTON))
    {
        m_eCurAnim = SWING;
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
