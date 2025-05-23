#include "Zombi.h"
#include "ItemRect.h"
#include "MCTerrain.h"
#include "MissionControl.h"

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
    m_fAttackDistance = 2.f;
    m_fSpeed = 1.5f;
    m_Hp = 50.f;
    m_MaxHp = 50.f;

    m_Coll_Size = { 0.3f, 1.f, 0.3f };
    m_Coll_Offset = { 0.f, 1.f, 0.f };

    __super::Initialize(pArg);

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Bone()))
        return E_FAIL;

    if (FAILED(Ready_Animation()))
        return E_FAIL;

    // 콜백 등록
    m_skelAnime->SetFrameCallback(std::bind(&CZombi::FrameCallback, this, std::placeholders::_1, std::placeholders::_2));

    return S_OK;
}

void CZombi::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CZombi::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
    
    Update_State(fTimeDelta);

}   

void CZombi::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
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
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Zombi"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    m_pVIBufferComs.resize(6);
    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Zombi_Body"),
        TEXT("m_pVIBufferCom_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[0]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Zombi_Head"),
        TEXT("m_pVIBufferCom_Head"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[1]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Zombi_Leg"),
        TEXT("m_pVIBufferCom_Foot_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[2]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Zombi_Leg"),
        TEXT("m_pVIBufferCom_Foot_L"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[3]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Zombi_Arm"),
        TEXT("m_pVIBufferCom_Arm_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[4]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Zombi_Arm"),
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
         { "Pelvis",  0,  MAtrixTranslation(0.f,  12.f / 16.f, 0.f),	MAtrixTranslation(0.f,   12.f / 16.f,	0.f), Matrix(), MAtrixTranslation(0, 6.f / 16.f, 0.f)},
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
    m_skelAnime->Update_Animetion(INIT, fTimeDelta, 0);
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

        _float3 vForce{ m_pTargetPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
        D3DXVec3Normalize(&vForce, &vForce);
        vForce *= 3.f;
        vForce.y += 4.f;

        m_pTargetPawn->Knock_back(vForce);
        m_pTargetPawn->Add_Hp(-15.f);
    }

    m_skelAnime->Update_Animetion(Attack, fTimeDelta, 5);
    m_skelAnime->Update_Animetion(Attack, fTimeDelta, 6);
}

void CZombi::Motion_Dead(_float fTimeDelta)
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
        m_pGameInstance->PopPool(this, TEXT("Layer_Monster"));


        _float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{ 0.0, 0.5, 0.0 };
        
        int x = static_cast<int>(Pos.x) / 16;
        int z = static_cast<int>(Pos.z) / 16;
        int width;
        if (CMCTerrain* _terrain = dynamic_cast<CMCTerrain*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Terrain")))) {
            width = static_cast<int>(sqrt(_terrain->Get_ChunkCount()));
        }
        int myChunk =  x + (width * z);
        wchar_t layerName[100];
        swprintf(layerName, 100, L"Layer_Chunk%d", myChunk);
        if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
            return;
        if (CItemRect* _copy = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
            _copy->SetPos(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
            _copy->Set_Bright(m_pTransformCom->Get_State(CTransform::STATE_POSITION).y);
            _copy->Set_ItemTypeAndBindTexture(ITEMNAME_ROTTENFLESH);
        }

        if (CMissionControl* _control = dynamic_cast<CMissionControl*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Mission")))) {
            _control->Update_Mission(L"zombi");
        }
    }
}

void CZombi::Turn(_float fTimeDelta)
{
}

void CZombi::Dead_Pawn()
{
    CPawn::Dead_Pawn();

    m_pGameInstance->Play_Sound(TEXT("Zombie_Death"), SOUND_DEAD, this, m_sound, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

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
}

void CZombi::FrameCallback(int animType, int frame)
{
    //std::cout << " 좀비 애니메이션: " << animType << ", 프레임: " << frame << std::endl;

    // 애니메이션 타입 ANIM_TYPE 
	// 애니메이션 타입 + 애니메이션 프레임에 맞는 행동을 하게끔 구현하면 됨
    // 공격 마지막 프레임(2) 은 버그있음 넉백때문에 마지막프레임에 안들어감

    if (animType == Swing_FF && frame == 2)
    {
        m_pGameInstance->Play_Sound(TEXT("Zombie_Walk1"), SOUND_WALK1, this, m_sound-0.35, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        int random = rand() % 10;
        if (random < 4) {
            switch (random)
            {
            case 0:
                m_pGameInstance->Play_Sound(TEXT("Zombie_Say1"), SOUND_SAY1, this, m_sound - 0.35, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                break;
            case 1:
                m_pGameInstance->Play_Sound(TEXT("Zombie_Say2"), SOUND_SAY2, this, m_sound - 0.35, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                break;
            case 2:
                m_pGameInstance->Play_Sound(TEXT("Zombie_Say3"), SOUND_SAY3, this, m_sound - 0.35, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                break;
            default:
                break;
            }
        }
    }
}
