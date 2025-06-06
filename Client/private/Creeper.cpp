﻿#include "Creeper.h"
#include "GameInstance.h"
#include "MCTerrain.h"
#include "BreakableRect.h"
#include "MissionControl.h"
#include "ItemRect.h"

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


    if (m_eCurAnim != ANIM::ATTACK)
        m_pGameInstance->Stop_Sound(SOUND_ATTACK, this);

    //m_pGameInstance->Check_Sound_Stop(this, m_eCurAnim,0);
}

void CCreeper::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
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
    mat3.Scaling(1.10f, 1.02f, 1.02f);

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
    mat10.Scaling(1.60f, 1.2f, 1.2f); 


    Matrix mat11 = {};
    mat11.Scaling(1.64f, 1.24f, 1.24f); //마지막 크기

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
    KEYFREAME Attack_11 = { 1.3f,  mat11 };


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
    m_skelAnime->Add_Animation(ANIM_type::Attack, Attack_11);


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

        const _float fDamage{ -40.f };
        const _float fRange{ 10.f };

        _float3 vForce{ m_pTargetPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION) };
        D3DXVec3Normalize(&vForce, &vForce);
        vForce *= 3.f;
        vForce.y += 4.f;

        m_pTargetPawn->Knock_back(vForce);
        m_pTargetPawn->Add_Hp(fDamage);

        auto GameObjects = m_pGameInstance->Get_GameObjectList(LEVEL_YU, TEXT("Layer_Monster"));
        for (CGameObject* pGameObject : GameObjects)
        {
            if (this == pGameObject)
                continue;

            CPawn* pPawn{ static_cast<CPawn*>(pGameObject) };

            _float3 vOtherPos = pPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION);
            _float3 vMyPos = Get_Transform()->Get_State(CTransform::STATE_POSITION);
            _float3 vDiff{ vOtherPos - vMyPos };

            
            if (D3DXVec3LengthSq(&vDiff) < fRange)
            {
                pPawn->Add_Hp(fDamage);
                pPawn->Knock_back(vForce);
            }
        }

        //주변 큐브 콜라이더 가져와서
		list<CCollider*> Colliders;
        CMCTerrain* pTerrain{ static_cast<CMCTerrain*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Terrain"))) };
        Colliders = pTerrain->Active_Near_Chunk_Colliders(
			m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f, 1.5f, 0.f },
            fRange
		);

		for (auto pCollider : Colliders)
		{
            // 범죄 on ㅋㅋ
            static_cast<CCube*>(pCollider->Get_Owner())->isCreeperBoom = true;

            if (CBreakableCube * pBreakableCube{ dynamic_cast<CBreakableCube*>(pCollider->Get_Owner()) })
            {
				CCollider_Cube* pCubeCollider{ dynamic_cast<CCollider_Cube*>(pCollider) };

				pBreakableCube->Delete_Cube(pCubeCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCubeCollider->Get_Offset());
            }
			else if (CBreakableRect * pBreakableRect{ dynamic_cast<CBreakableRect*>(pCollider->Get_Owner()) })
			{
                CCollider_Cube* pCubeCollider{ dynamic_cast<CCollider_Cube*>(pCollider) };
				pBreakableRect->Delete_Cube(pCubeCollider->Get_Transform()->Get_State(CTransform::STATE_POSITION) + pCubeCollider->Get_Offset());
			}
		}
        //주변 큐브 콜라이더 해제
		for (auto pCollider : Colliders)
		{
			m_pGameInstance->Out_Collider_CollisiomGroup(COLLISION_BLOCK, pCollider);
		}

        // 소리 재생 초기화
        //m_pGameInstance->Check_Sound_Stop(this, 0,0);
        m_pGameInstance->PopPool(this, TEXT("Layer_Monster"));
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
        m_pGameInstance->PopPool(this, TEXT("Layer_Monster"));

        _float3 Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + _float3{0.0, 0.5, 0.0};

        int x = static_cast<int>(Pos.x) / 16;
        int z = static_cast<int>(Pos.z) / 16;
        int width;
        if (CMCTerrain* _terrain = dynamic_cast<CMCTerrain*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Terrain")))) {
            width = static_cast<int>(sqrt(_terrain->Get_ChunkCount()));
        }
        int myChunk = x + (width * z);
        wchar_t layerName[100];
        swprintf(layerName, 100, L"Layer_Chunk%d", myChunk);
        if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_YU, TEXT("Prototype_GameObject_ItemRect"), LEVEL_YU, layerName)))
            return;
        if (CItemRect* _copy = dynamic_cast<CItemRect*>(m_pGameInstance->Get_LastObject(LEVEL_YU, layerName))) {
            _copy->SetPos(Pos);
            _copy->Set_Bright(m_pTransformCom->Get_State(CTransform::STATE_POSITION).y);
            _copy->Set_ItemTypeAndBindTexture(ITEMNAME_GUNPOWDER);
        }

        if (CMissionControl* _control = dynamic_cast<CMissionControl*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Mission")))) {
            _control->Update_Mission(L"creeper");
        }
    }
}

void CCreeper::Turn(_float fTimeDelta)
{
}

void CCreeper::Dead_Pawn()
{
    CPawn::Dead_Pawn();
    m_pGameInstance->Play_Sound(TEXT("Creeper_Death"), SOUND_DEAD, this, m_sound, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

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
    if (animType == Attack && frame == 0)
    {
        //std::cout << " 크리퍼 애니메이션: " << animType << ", 프레임: " << frame << std::endl;
        m_pGameInstance->Play_Sound(TEXT("Creeper_Explosion"), SOUND_ATTACK, this, m_sound+0.1f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
    }
}

