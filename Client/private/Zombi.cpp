#include "Zombi.h"

CZombi::CZombi(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{
}

CZombi::CZombi(const CZombi& Prototype)
    : CMonster(Prototype)
{
}

HRESULT CZombi::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CZombi::Initialize(void* pArg)
{
    m_MonsterType = MT_Zombie;
    m_fAttackDistance = 1.f;

    __super::Initialize(pArg);

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Bone()))
        return E_FAIL;

    if (FAILED(Ready_Animation()))
        return E_FAIL;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(2.f, 0.f, 0.f));


    return S_OK;
}

void CZombi::Priority_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_CollisionGroup(COLLISION_MONSTER, this);
}

void CZombi::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta); Attack;
}

void CZombi::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Key_Down('Q'))
    {
        m_eCurAnim = ATTACK;
    }

    Update_State(fTimeDelta);
    m_skelAnime->Update_RootBone(*m_pTransformCom->Get_WorldMatrix());

    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
        return;
}

HRESULT CZombi::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT CZombi::Ready_Components()
{
#pragma region 외형
    // 좀비 텍스처
  /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_Texture_Zombi"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    m_pVIBufferComs.resize(6);
    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Zombi_Body"),
        TEXT("m_pVIBufferCom_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[0]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Zombi_Head"),
        TEXT("m_pVIBufferCom_Head"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[1]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Zombi_Leg"),
        TEXT("m_pVIBufferCom_Foot_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[2]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Zombi_Leg"),
        TEXT("m_pVIBufferCom_Foot_L"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[3]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Zombi_Arm"),
        TEXT("m_pVIBufferCom_Arm_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[4]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_HERO, TEXT("Prototype_Component_VIBuffer_Zombi_Arm"),
        TEXT("m_pVIBufferCom_Arm_L"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[5]))))
        return E_FAIL;
#pragma endregion

    if (FAILED(__super::Ready_Components()))
        return E_FAIL;

    return S_OK;
}

HRESULT CZombi::Ready_Bone()
{
    Matrix Arm_R = MAtrixTranslation(6.f / 16.f, 12.f / 16.f, 0.f);
    Arm_R.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-80));

    Matrix Arm_L = MAtrixTranslation(-6.f / 16.f, 12.f / 16.f, 0.f);
    Arm_L.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-80));

    BONE bone[7] =
    {
         { "Root"  , -1,  MAtrixTranslation(0.f, 0.f,0.f),	MAtrixTranslation(0.f	,0.f,	0.f),	Matrix(), Matrix() },  // root
         { "Pelvis",  0,  MAtrixTranslation(0.f,  12.f / 16.f,0.f),	MAtrixTranslation(0.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, 6.f / 16.f, 0.f)},
         { "Neck"  ,  1,  MAtrixTranslation(0.f,  12.f / 16.f,0.f),	MAtrixTranslation(0.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, 4.f / 16.f, 0.f)},
         { "Leg_R",  1,	  MAtrixTranslation(2.f / 16.f,  0.f / 16.f,	0.f),	MAtrixTranslation(2.f / 16.f,	 0,		0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
         { "Leg_L" ,  1,  MAtrixTranslation(-2.f / 16.f,  0.f / 16.f,	0.f),	MAtrixTranslation(-2.f / 16.f,     0,	0.f), Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
         { "Arm_R" ,  1,  Arm_R, Arm_R, Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
         { "Arm_L" ,  1,  Arm_L, Arm_L, Matrix(), MAtrixTranslation(0, -6.f / 16.f, 0.f)},
    };

    for (int i = 0; i < 7; i++)
    {
        m_skelAnime->Add_Bone(bone[i]);
    }

    return S_OK;
}

HRESULT CZombi::Ready_Animation()
{
/*----------
* INIT 모션
------------*/
    Matrix mat = {};
    KEYFREAME Init = { 0.f, mat };
    m_skelAnime->Add_Animation(ANIM_type::INIT, Init);

    /*----------
    * Walk 모션
    ------------*/
    Matrix mat2 = {};
    mat2.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(50.f));

    Matrix mat3 = {};
    mat3.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-50.f));

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

    m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_1_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_2_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_3_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_4_B);
    m_skelAnime->Add_Animation(ANIM_type::Swing_BF, Walk_5_B);


   /*----------
   * Attack 모션
   ------------*/

    mat2 = {};
    mat2.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-30));

    KEYFREAME Attack_1 = { 0.f, mat }; //0
    KEYFREAME Attack_2 = { 0.5f, mat2 };
    KEYFREAME Attack_3 = { 1.f, mat };

    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_1);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_2);
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_3);


    /*----------
    Dead 모션
    --------*/
    mat2 = {};
    mat2.Turn_Radian(_float3(1.f, 0.f, 1.f), D3DXToRadian(-90));

    KEYFREAME Dead_1 = { 0.0f,   mat };
    KEYFREAME Dead_2 = { 0.8f,   mat2 };

    m_skelAnime->Add_Animation(ANIM_type::Dead, Dead_1);
    m_skelAnime->Add_Animation(ANIM_type::Dead, Dead_2);

    return S_OK;
}

void CZombi::Update_State(_float fTimeDelta)
{
    switch (m_eCurAnim)
    {
    case CZombi::IDLE:
        Motion_Idle(fTimeDelta);
        break;
    case CZombi::WALK:
        Motion_Walk(fTimeDelta);
        break;
    case  CZombi::ATTACK:
        Motion_Attack(fTimeDelta);
        break;
    case  CZombi::DEAD:
        Motion_Dead(fTimeDelta);
        break;
    case CZombi::ANIM_END:
        break;
    default:
        break;
    }
}

void CZombi::Motion_Idle(_float fTimeDelta)
{
    if (m_skelAnime->is_AnimtionEND(INIT))
    {
        m_eCurAnim = IDLE;
    }
}

void CZombi::Motion_Walk(_float fTimeDelta)
{
    if (m_skelAnime->is_AnimtionEND(Swing_BF) &&
        m_skelAnime->is_AnimtionEND(Swing_FF))
    {
        m_eCurAnim = WALK;
    }

    m_skelAnime->Update_Animetion(Swing_BF, fTimeDelta, 3);
    m_skelAnime->Update_Animetion(Swing_FF, fTimeDelta, 4);
}

void CZombi::Motion_Attack(_float fTimeDelta)
{
    if (m_skelAnime->is_AnimtionEND(Attack))
    {
        m_eCurAnim = IDLE;
    }

    m_skelAnime->Update_Animetion(Attack, fTimeDelta, 5);
    m_skelAnime->Update_Animetion(Attack, fTimeDelta, 6);
}

void CZombi::Motion_Dead(_float fTimeDelta)
{
    m_skelAnime->Update_Animetion(Dead, fTimeDelta, 0);

    if (m_skelAnime->is_AnimtionEND(Dead))
    {
        // 맞아 죽었을 때 
        m_eCurAnim = IDLE;
    }
}

void CZombi::Turn(_float fTimeDelta)
{
}

CZombi* CZombi::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CZombi* pInstance = new CZombi(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CZombi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CZombi::Clone(void* pArg)
{
    CZombi* pInstance = new CZombi(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CZombi");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CZombi::Free()
{
    __super::Free();
    Safe_Release(m_pTextureCom);

}

