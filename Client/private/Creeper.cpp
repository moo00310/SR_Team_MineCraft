#include "Creeper.h"

#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"

CCreeper::CCreeper(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{
}

CCreeper::CCreeper(const CCreeper& Prototype)
    : CMonster(Prototype)
{
}

HRESULT CCreeper::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCreeper::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Bone()))
        return E_FAIL;

    if (FAILED(Ready_Animation()))
        return E_FAIL;

    return S_OK;
}

void CCreeper::Priority_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_CollisionGroup(COLLISION_MONSTER, this);
}

void CCreeper::Update(_float fTimeDelta)
{ 
    __super::Update(fTimeDelta);

    //m_pCollider_CubeCom->Update_ColliderBox();

    //CGameObject* pHitObject{ nullptr };
    //_float fDist{};

    //pHitObject = m_pGameInstance->Ray_Cast
    //(   m_pTransformCom->Get_State(CTransform::STATE_POSITION),
    //    m_pTransformCom->Get_State(CTransform::STATE_LOOK),
    //    10.f,
    //    COLLISION_PLAYER,
    //    fDist
    //);

}

void CCreeper::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Key_Down('Q'))
    {
        m_eCurAnim = WALK;
    }

    Update_State(fTimeDelta);
    m_skelAnime->Update_RootBone(*m_pTransformCom->Get_WorldMatrix());

    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
        return;
}

HRESULT CCreeper::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    for (int i = 0; i < 6; i++)
    {
        if (FAILED(m_pVIBufferComs[i]->Bind_WorldMatrix()))
            return E_FAIL;

        if (FAILED(m_pVIBufferComs[i]->Bind_Buffers()))
            return E_FAIL;

        if (FAILED(m_pVIBufferComs[i]->Render()))
            return E_FAIL;
    }

    //m_pCollider_CubeCom->Render_ColliderBox(false);

    return S_OK;
}

HRESULT CCreeper::Ready_Components()
{
    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 3.f, D3DXToRadian(30.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

#pragma region 외형
    // 크리퍼 텍스처
   /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Creeper"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    m_pVIBufferComs.resize(6);
    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Creeper_Body"),
        TEXT("m_pVIBufferCom_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[0]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Creeper_Head"),
        TEXT("m_pVIBufferCom_Head"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[1]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
        TEXT("m_pVIBufferCom_Foot_LF"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[2]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
        TEXT("m_pVIBufferCom_Foot_RF"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[3]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
        TEXT("m_pVIBufferCom_Foot_LB"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[4]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
        TEXT("m_pVIBufferCom_Foot_RB"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[5]))))
        return E_FAIL;
#pragma endregion

    // 본 + 애니메이션
    CSkeletalAnimator::DESC DescSekel = { m_pVIBufferComs };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SkeletalAnimator"),
        TEXT("m_pSkeletalAnimatorCom"), reinterpret_cast<CComponent**>(&m_skelAnime), &DescSekel)))
        return E_FAIL;


    ///* For.Com_Collider_Cube */
    //CCollider_Cube::COLLCUBE_DESC		ColliderCubeDesc;
    //ColliderCubeDesc.fRadiusY = 1.f;
    //ColliderCubeDesc.fOffSetY = 1.f;
    //ColliderCubeDesc.pTransformCom = m_pTransformCom;
    //if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
    //    TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pCollider_CubeCom), &ColliderCubeDesc)))
    //    return E_FAIL;

    return S_OK;
}

HRESULT CCreeper::Ready_Bone()
{
    BONE bone[7] =
    {
         { "Root"  , -1,   MAtrixTranslation(0.f, 0.f,0.f),	MAtrixTranslation(0.f	,0.f,	0.f), Matrix(), Matrix() },  // root
         { "Pelvis",  0,   MAtrixTranslation(0.f,  6.f / 16.f ,0.f),	MAtrixTranslation(0.f,   6.f / 16.f, 0.f), Matrix(), MAtrixTranslation(0.f,  6.f / 16.f, 0.f)},
         { "Neck"   ,  1,  MAtrixTranslation(0.f,  12.f / 16.f ,0.f), MAtrixTranslation(0.f,  12.f / 16.f ,0.f), Matrix(), MAtrixTranslation(0.f,  4.f / 16.f ,0.f), },
         { "Leg_LF" ,  1,  MAtrixTranslation(2.f / 16.f, 0,  -2.f / 16.f), MAtrixTranslation(2.f / 16.f, 0,  -2.f / 16.f), Matrix(), MAtrixTranslation(0, -3.f / 16.f,  -2.f / 16.f)},
         { "Leg_RF" ,  1,  MAtrixTranslation(-2.f / 16.f,  0,  -2.f / 16.f),  MAtrixTranslation(-2.f / 16.f,  0,  -2.f / 16.f), Matrix(), MAtrixTranslation(0, -3.f / 16.f,  -2.f / 16.f)},
         { "Leg_LB" ,  1,  MAtrixTranslation(2.f / 16.f,  0, 2.f / 16.f),MAtrixTranslation(2.f / 16.f,  0, 2.f / 16.f), Matrix(), MAtrixTranslation(0, -3.f / 16.f,  2.f / 16.f)},
         { "Leg_RB" ,  1,  MAtrixTranslation(-2.f / 16, 0, 2.f / 16.f), MAtrixTranslation(-2.f / 16, 0, 2.f / 16.f), Matrix(), MAtrixTranslation(0, -3.f / 16.f,  2.f / 16.f)},
    };

    for (int i = 0; i < 7; i++)
    {
        m_skelAnime->Add_Bone(bone[i]);
    }

    return S_OK;
}

HRESULT CCreeper::Ready_Animation()
{
    /*----------
    * INIT 모션
    - -----------*/
    Matrix mat = {};
    KEYFREAME Init = { 0.f, mat };
    m_skelAnime->Add_Animation(ANIM_type::INIT, Init);

    /*----------
    * Walk 모션
    ------------*/
    Matrix mat2 = {};
    mat2.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(25.f));

    Matrix mat3 = {};
    mat3.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-25.f));

    KEYFREAME Walk_1_F = { 0.f, mat }; //0
    KEYFREAME Walk_2_F = { 0.5f, mat2 }; //60
    KEYFREAME Walk_3_F = { 1.0f, mat }; // 0
    KEYFREAME Walk_4_F = { 1.5f, mat3 }; // -60
    KEYFREAME Walk_5_F = { 2.0f, mat }; // 0

    KEYFREAME Walk_1_B = { 0.f,  mat };
    KEYFREAME Walk_2_B = { 0.5f, mat3 };
    KEYFREAME Walk_3_B = { 1.0f, mat };
    KEYFREAME Walk_4_B = { 1.5f, mat2 };
    KEYFREAME Walk_5_B = { 2.f, mat };

    m_skelAnime->Add_Animation(ANIM_type::Swing_FF, Walk_1_F);
    m_skelAnime->Add_Animation(ANIM_type::Swing_FF, Walk_2_F);
    m_skelAnime->Add_Animation(ANIM_type::Swing_FF, Walk_3_F);
    m_skelAnime->Add_Animation(ANIM_type::Swing_FF, Walk_4_F);
    m_skelAnime->Add_Animation(ANIM_type::Swing_FF, Walk_5_F);

    m_skelAnime->Add_Animation(ANIM_type::Swing_FA, Walk_1_F);
    m_skelAnime->Add_Animation(ANIM_type::Swing_FA, Walk_2_F);
    m_skelAnime->Add_Animation(ANIM_type::Swing_FA, Walk_3_F);
    m_skelAnime->Add_Animation(ANIM_type::Swing_FA, Walk_4_F);
    m_skelAnime->Add_Animation(ANIM_type::Swing_FA, Walk_5_F);

    m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_1_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_2_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_3_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_4_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_5_B);

    m_skelAnime->Add_Animation(ANIM_type::Swing_BA, Walk_1_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BA, Walk_2_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BA, Walk_3_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BA, Walk_4_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BA, Walk_5_B);

    return S_OK;
}

void CCreeper::Update_State(_float fTimeDelta)
{
    switch (m_eCurAnim)
    {
    case CCreeper::IDLE:
        Motion_Idle(fTimeDelta);
        break;
    case CCreeper::WALK:
        Motion_Walk(fTimeDelta);
        break;
    case CCreeper::ANIM_END:
        break;
    default:
        break;
    }
}

void CCreeper::Motion_Idle(_float fTimeDelta)
{
}

void CCreeper::Motion_Walk(_float fTimeDelta)
{
    if (m_skelAnime->is_AnimtionEND(Swing_BF) &&
        m_skelAnime->is_AnimtionEND(Swing_FF) &&
        m_skelAnime->is_AnimtionEND(Swing_BA) &&
        m_skelAnime->is_AnimtionEND(Swing_FA)
        )
    {
        m_eCurAnim = WALK;
    }

    m_skelAnime->Update_Animetion(Swing_BF, fTimeDelta, 3);
    m_skelAnime->Update_Animetion(Swing_FF, fTimeDelta, 4);
    m_skelAnime->Update_Animetion(Swing_BA, fTimeDelta, 6);
    m_skelAnime->Update_Animetion(Swing_FA, fTimeDelta, 5);
}

void CCreeper::Turn(_float fTimeDelta)
{
}

CCreeper* CCreeper::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CCreeper* pInstance = new CCreeper(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CCreeper");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCreeper::Clone(void* pArg)
{
    CCreeper* pInstance = new CCreeper(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CCreeper");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCreeper::Free()
{
    __super::Free();

    //Safe_Release(m_pCollider_CubeCom);
    Safe_Release(m_pTextureCom);

}

