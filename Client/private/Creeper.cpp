#include "Creeper.h"
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
    m_MonsterType = MT_Creeper;
    m_fAttackDistance = 3.f;
    m_fSpeed = 1.5f;
    m_Hp = 100.f;
    m_MaxHp = 100.f;

    m_Coll_Size = { 0.3f, 0.8f, 0.3f };
    m_Coll_Offset = { 0.f, 0.8f, 0.f };

    __super::Initialize(pArg); 

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Bone()))
        return E_FAIL;

    if (FAILED(Ready_Animation()))
        return E_FAIL;

    // 콜백 등록
    m_skelAnime->SetFrameCallback(std::bind(&CCreeper::FrameCallback, this, std::placeholders::_1, std::placeholders::_2));


    return S_OK;
}

void CCreeper::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    m_pGameInstance->Add_CollisionGroup(COLLISION_MONSTER, this);
}

void CCreeper::Update(_float fTimeDelta)
{ 
    if (m_eCurAnim == ATTACK) {
        if (m_renderColor) {
            m_eColor = RENDERORIGIN;
        }
        else {
            m_eColor = RENDERBOOM;
        }
        m_renderColor = !m_renderColor;
    }

    __super::Update(fTimeDelta);

    Update_State(fTimeDelta);
}

void CCreeper::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.5f))
    {
        m_skelAnime->Update_RootBone(*m_pTransformCom->Get_WorldMatrix());

        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }
}

HRESULT CCreeper::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT CCreeper::Ready_Components()
{
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

    //버퍼 연결 하고 Transform과 AinmaitonCom 연결
    if (FAILED(__super::Ready_Components()))
        return E_FAIL;

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

/*----------
* Attack 모션
------------*/
    // MAX =   Scaling(1.60f, 1.2f, 1.2f);

    mat2 = {};
    mat2.Scaling(1.12f, 1.04f, 1.04f); // 성장 1

    mat3 = {};
    mat3.Scaling(1.10, 1.02f, 1.02f);

    Matrix mat4 = {};
    mat4.Scaling(1.24f, 1.08f, 1.08f);  // 성장 2

    Matrix mat5 = {};
    mat5.Scaling(1.22f, 1.06f, 1.06f);

    Matrix mat6 = {};
    mat6.Scaling(1.36f, 1.12f, 1.12f);  // 성장 3

    Matrix mat7 = {};
    mat7.Scaling(1.34f, 1.10f, 1.10f);

    Matrix mat8 = {};
    mat8.Scaling(1.48f, 1.16f, 1.16f); // 성장 4

    Matrix mat9 = {};
    mat9.Scaling(1.46f, 1.14f, 1.14f); 

    Matrix mat10 = {};
    mat10.Scaling(1.60f, 1.2f, 1.2f); //마지막 크기

    KEYFREAME Attack_1 = { 0.0f,   mat };
    KEYFREAME Attack_2 = { 0.2f,   mat2 };
    KEYFREAME Attack_3 = { 0.25f,  mat3 };
    KEYFREAME Attack_4 = { 0.4f,   mat4 };
    KEYFREAME Attack_5 = { 0.45f,  mat5 };
    KEYFREAME Attack_6 = { 0.6f,   mat6 };
    KEYFREAME Attack_7 = { 0.65f,  mat7 };
    KEYFREAME Attack_8 = { 0.8f,   mat8 };
    KEYFREAME Attack_9 = { 0.85f,  mat9 };
    KEYFREAME Attack_10 = { 1.0f,  mat10 };


    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_1);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_2);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_3);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_4);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_5);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_6);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_7);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_8);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_9);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_10);


/*----------
* Dead 모션
------------*/

    mat2 = {};
    mat2.Turn_Radian(_float3(1.f, 0.f, 1.f), D3DXToRadian(-90));

    KEYFREAME Dead_1 = { 0.0f,   mat };
    KEYFREAME Dead_2 = { 0.8f,   mat2 };

    m_skelAnime->Add_Animation(ANIM_type::Dead, Dead_1);
    m_skelAnime->Add_Animation(ANIM_type::Dead, Dead_2);


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
    case  CCreeper::ATTACK:
        Motion_Attack(fTimeDelta);
        break;
    case  CCreeper::DEAD:
        Motion_Dead(fTimeDelta);
        break;
    case CCreeper::ANIM_END:
        break;
    default:
        break;
    }
}

void CCreeper::Motion_Idle(_float fTimeDelta)
{
    m_skelAnime->Update_Animetion(INIT, fTimeDelta, 0);
    m_skelAnime->Update_Animetion(INIT, fTimeDelta, 1);
    m_skelAnime->Update_Animetion(INIT, fTimeDelta, 2);
    m_skelAnime->Update_Animetion(INIT, fTimeDelta, 3);
    m_skelAnime->Update_Animetion(INIT, fTimeDelta, 4);
    m_skelAnime->Update_Animetion(INIT, fTimeDelta, 5);
    m_skelAnime->Update_Animetion(INIT, fTimeDelta, 6);
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

void CCreeper::Motion_Attack(_float fTimeDelta)
{
    m_skelAnime->Update_Animetion(Attack, fTimeDelta, 0);
    
    if (m_skelAnime->is_AnimtionEND(Attack))
    {
        // 폭발 파티클.
        CParticleEventManager::Get_Instance()->OnParticle(
            PROTOTYPE_GAMEOBJECT_PARTICLE_EXPLOSION,
            m_pTransformCom
        );

        m_eCurAnim = BOOM;

        _float3 temp = m_pTargetPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
        m_pTargetPawn->Knock_back(temp);
        //m_pTargetPawn->Set_Hp(95);
    }

}

void CCreeper::Motion_Dead(_float fTimeDelta)
{
    m_skelAnime->Update_Animetion(Dead, fTimeDelta, 0);

    // 사망 파티클.
    CParticleEventManager::Get_Instance()->OnParticle(
        PROTOTYPE_GAMEOBJECT_PARTICLE_DIE,
        m_pTransformCom,
        0.5f
    );

    if (m_skelAnime->is_AnimtionEND(Dead))
    {
        //m_isDestroyed = true;
        m_pGameInstance->PlaySound(TEXT("Creeper_Death"), 1, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        SetActive(false);
    }
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
}

void CCreeper::FrameCallback(int animType, int frame)
{
    //std::cout << " 크리퍼 애니메이션: " << animType << ", 프레임: " << frame << std::endl;
}

