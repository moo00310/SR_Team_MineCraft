#include "Warden.h"

#include "BTTask_DetectEnemy.h"
#include "BTTask_Chase.h"
#include "BTTask_Patrol.h"
#include "BTDecorator_IsTargetNear.h"
#include "BTTask_Attack.h"
#include "BTDistanceBranch.h"

CWarden::CWarden(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CWarden::CWarden(const CWarden& Prototype)
	: CMonster ( Prototype )
{
}

HRESULT CWarden::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWarden::Initialize(void* pArg)
{
    m_MonsterType = MT_WARDEN;
    m_fAttackDistance = 5.f;
    m_fSpeed = 1.5f;
    m_Hp = 100.f;
    m_MaxHp = 100.f;

    m_Coll_Size = { 0.5f, 1.5f, 0.5f };
    m_Coll_Offset = { 0.f, 1.1f, 0.f };

    __super::Initialize(pArg);

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Bone()))
        return E_FAIL;

    if (FAILED(Ready_Animation()))
        return E_FAIL;

    // 콜백 등록
    m_skelAnime->SetFrameCallback(std::bind(&CWarden::FrameCallback, this, std::placeholders::_1, std::placeholders::_2));

    m_pTransformCom->Set_State(CTransform::STATE_POSITION,_float3(10, 15, 10));
	return S_OK;
}

void CWarden::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    m_pGameInstance->Add_CollisionGroup(COLLISION_MONSTER, this);
}

void CWarden::Update(_float fTimeDelta)
{
 
    __super::Update(fTimeDelta);

    Update_State(fTimeDelta);

    m_pGameInstance->CheckSoundStop(this, m_eCurAnim, 0);
}

void CWarden::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.5f))
    {

        m_skelAnime->Update_RootBone(*m_pTransformCom->Get_WorldMatrix());

        if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
            return;
    }
}

HRESULT CWarden::Render()
{

    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    for (int i = 0; i < 6; i++)
    {
        if (FAILED(m_pVIBufferComs[i]->Bind_WorldMatrix(m_pShaderCom)))
            return E_FAIL;

        if (FAILED(m_pVIBufferComs[i]->Bind_Buffers()))
            return E_FAIL;

        m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);
        m_pShaderCom->SetFloat("g_Bright", g_fBright + 0.2f);
        m_pShaderCom->Begin(m_eColor);

        /* 정점을 그린다. */
        if (FAILED(m_pVIBufferComs[i]->Render()))
            return E_FAIL;

        m_pShaderCom->End();
    }

  // 뿔
    for (int i = 6; i < m_pVIBufferComs.size(); i++)
    {
        if (FAILED(m_pVIBufferComs[i]->Bind_WorldMatrix(m_pShaderCom)))
            return E_FAIL;

        if (FAILED(m_pVIBufferComs[i]->Bind_Buffers()))
            return E_FAIL;


        m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);
        m_pShaderCom->SetFloat("g_Bright", g_fBright + 0.2f);
        m_pShaderCom->Begin(2);

        /* 정점을 그린다. */
        if (FAILED(m_pVIBufferComs[i]->Render()))
            return E_FAIL;

        m_pShaderCom->End();
    }

    if (FAILED(m_pCollider_CubeCom->Render_Collider(true)))
        return E_FAIL;

    return S_OK;
}

HRESULT CWarden::Ready_Components()
{
#pragma region 외형
    // 워든 텍스쳐
   /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_Texture_Warden"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    m_pVIBufferComs.resize(8);
    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Warden_Body"),
        TEXT("m_pVIBufferCom_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[0]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Warden_Head"),
        TEXT("m_pVIBufferCom_Head"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[1]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Warden_Leg_R"),
        TEXT("m_pVIBufferCom_Leg_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[2]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Warden_Leg_L"),
        TEXT("m_pVIBufferCom_Leg_L"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[3]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Warden_Arm_R"),
        TEXT("m_pVIBufferCom_Arm_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[4]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Warden_Arm_L"),
        TEXT("m_pVIBufferCom_Arm_L"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[5]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Warden_Horn_R"),
        TEXT("m_pVIBufferCom_Horn_R"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[6]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_Warden_Horn_L"),
        TEXT("m_pVIBufferCom_Horn_L"), reinterpret_cast<CComponent**>(&m_pVIBufferComs[7]))))
        return E_FAIL;

#pragma endregion

    //버퍼 연결 하고 Transform과 AinmaitonCom 연결
    if (FAILED(__super::Ready_Components()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWarden::Ready_Bone()
{
    Matrix mat = {};
    mat.Scaling(0.5f, 0.5f, 0.5f);

    BONE bone[9] =
    {
         { "Root"  , -1,   MAtrixTranslation(0.f, 0.f,0.f),	MAtrixTranslation(0.f	,0.f,	0.f), Matrix(), Matrix() },  // root
         { "Pelvis",  0,   MAtrixTranslation(0.f,  6.f / 16.f ,0.f),	 MAtrixTranslation(0.f,  6.f / 16.f ,0.f), Matrix(), MAtrixTranslation(0.f,  10.5f/16.f, 0.f)},
         { "Neck"  ,  1,  MAtrixTranslation(0.f,  21.f/16.f ,1.f/16.f), MAtrixTranslation(0.f,  21.f / 16.f ,1.f / 16.f), Matrix(), MAtrixTranslation(0.f,  8.f /16.f ,0.f), },
         { "Leg_R" ,  0,  MAtrixTranslation(6.f / 16.f, 6.f / 16.f,  0.f),  MAtrixTranslation(6.f / 16.f, 6.f / 16.f,  0.f), Matrix(),MAtrixTranslation(0.f,  -6.5f / 16.f ,0.f)},
         { "Leg_L" ,  0,  MAtrixTranslation(-6.f / 16.f, 6.f / 16.f,  0.f),   MAtrixTranslation(-6.f / 16.f, 6.f / 16.f,  0.f), Matrix(), MAtrixTranslation(0.f,  -6.5f / 16.f ,0.f)},
         { "Arm_R" ,  1,  MAtrixTranslation(13.f / 16.f,  21.f/16.f, 0.f),MAtrixTranslation(13.f / 16.f,  21.f / 16.f, 0.f), Matrix(), MAtrixTranslation(0, -14.f / 16.f,  0.f)},
         { "Arm_L" ,  1,  MAtrixTranslation(-13.f / 16, 21.f/16.f,  0.f), MAtrixTranslation(-13.f / 16, 21.f / 16.f,  0.f), Matrix(), MAtrixTranslation(0, -14.f / 16.f,  0.f)},
         { "Horn_R" ,  2, MAtrixTranslation(24.f / 16, 28.f/16.f,  0.f) * mat, MAtrixTranslation(24.f / 16, 28.f / 16.f,  0.f) * mat, Matrix(), Matrix()},
         { "Horn_L" ,  2, MAtrixTranslation(-24.f / 16, 28.f/16.f,  0.f) * mat, MAtrixTranslation(-24.f / 16, 28.f / 16.f,  0.f) * mat, Matrix(),Matrix()},
    };

    for (int i = 0; i < 9; i++)
    {
        m_skelAnime->Add_Bone(bone[i]);
    }

    return S_OK;
}

HRESULT CWarden::Ready_Animation()
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
    mat2.Turn_Radian(_float3(1.f, -0.1f, 0.2f), D3DXToRadian(20.f));

    Matrix mat6 = {};
    mat6.Turn_Radian(_float3(0.f, 0.2f, -0.15f), D3DXToRadian(15.f));

    Matrix mat3 = {};
    mat3.Turn_Radian(_float3(1.f, 0.15f, -0.2f), D3DXToRadian(-20.f));


    Matrix mat4 = {};
    mat4.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-50.f));

    Matrix mat5 = {};
    mat5.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(50.f));

    KEYFREAME Pelvis = { 0.f, mat }; //0
    KEYFREAME Pelvis1 = { 1.f, mat2 }; // 25
    KEYFREAME Pelvis2= { 2.f, mat6 }; // 0
    KEYFREAME Pelvis3 = { 3.f, mat3 }; // -25
    KEYFREAME Pelvis4 = { 4.f, mat }; // 0

    KEYFREAME Leg_R1 = { 0.f, mat }; //0
    KEYFREAME Leg_R2 = { 0.5f, mat4 }; // 
    KEYFREAME Leg_R3 = { 1.f, mat }; // 0
    KEYFREAME Leg_R4 = { 1.5f, mat5 }; // 
    KEYFREAME Leg_R5 = { 2.f, mat }; // 0
    KEYFREAME Leg_R6 = { 2.5f, mat4 }; // 0
    KEYFREAME Leg_R7 = { 3.f, mat }; // 0
    KEYFREAME Leg_R8 = { 3.5f, mat5 }; // 0
    KEYFREAME Leg_R9 = { 4.f, mat }; // 0

    KEYFREAME Leg_L1 = { 0.f, mat }; 
    KEYFREAME Leg_L2 = { 0.5f, mat5 };  
    KEYFREAME Leg_L3 = { 1.f, mat }; 
    KEYFREAME Leg_L4 = { 1.5f, mat4 };  
    KEYFREAME Leg_L5 = { 2.f, mat }; 
    KEYFREAME Leg_L6 = { 2.5f, mat5 };
    KEYFREAME Leg_L7 = { 3.f, mat }; 
    KEYFREAME Leg_L8 = { 3.5f, mat4 };
    KEYFREAME Leg_L9 = { 4.f, mat }; 


    m_skelAnime->Add_Animation(ANIM_type::Swing_Pevis, Pelvis);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Pevis, Pelvis1);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Pevis, Pelvis2);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Pevis, Pelvis3);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Pevis, Pelvis4);

    m_skelAnime->Add_Animation(ANIM_type::Swing_Leg_R, Leg_R1);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Leg_R, Leg_R2);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Leg_R, Leg_R3);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Leg_R, Leg_R4);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Leg_R, Leg_R5);

    m_skelAnime->Add_Animation(ANIM_type::Swing_Leg_L, Leg_L1);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Leg_L, Leg_L2);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Leg_L, Leg_L3);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Leg_L, Leg_L4);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Leg_L, Leg_L5);

    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_R, Leg_L1);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_R, Leg_L2);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_R, Leg_L3);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_R, Leg_L4);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_R, Leg_L5);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_R, Leg_L6);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_R, Leg_L7);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_R, Leg_L8);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_R, Leg_L9);

    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_L, Leg_R1);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_L, Leg_R2);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_L, Leg_R3);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_L, Leg_R4);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_L, Leg_R5);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_L, Leg_R6);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_L, Leg_R7);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_L, Leg_R8);
    m_skelAnime->Add_Animation(ANIM_type::Swing_Arm_L, Leg_R9);
   

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

void CWarden::Update_State(_float fTimeDelta)
{
    switch (m_eCurAnim)
    {
    case CWarden::IDLE:
        Motion_Idle(fTimeDelta);
        break;
    case CWarden::WALK:
        Motion_Walk(fTimeDelta);
        break;
    case  CWarden::ATTACK:
        Motion_Attack(fTimeDelta);
        break;
    case  CWarden::DEAD:
        Motion_Dead(fTimeDelta);
        break;
    case CWarden::ANIM_END:
        break;
    default:
        break;
    }
}

void CWarden::Motion_Idle(_float fTimeDelta)
{
    m_skelAnime->Update_Animetion(INIT, fTimeDelta, 0);
}

void CWarden::Motion_Walk(_float fTimeDelta)
{
    if (m_skelAnime->is_AnimtionEND(Swing_Leg_R) &&
        m_skelAnime->is_AnimtionEND(Swing_Leg_L) &&
        m_skelAnime->is_AnimtionEND(Swing_Pevis) &&
        m_skelAnime->is_AnimtionEND(Swing_Arm_R) &&
        m_skelAnime->is_AnimtionEND(Swing_Arm_L)
        )
    {
        m_eCurAnim = WALK;
    }
    
    m_skelAnime->Update_Animetion(Swing_Pevis, fTimeDelta, 1);
    m_skelAnime->Update_Animetion(Swing_Leg_R, fTimeDelta, 3);
    m_skelAnime->Update_Animetion(Swing_Leg_L, fTimeDelta, 4);

    m_skelAnime->Update_Animetion(Swing_Arm_R, fTimeDelta, 5);
    m_skelAnime->Update_Animetion(Swing_Arm_L, fTimeDelta, 6);
}

void CWarden::Motion_Attack(_float fTimeDelta)
{
    if (m_skelAnime->is_AnimtionEND(Attack))
    {

        _float3 temp = m_pTargetPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
        m_pTargetPawn->Knock_back(temp);
    }

    m_skelAnime->Update_Animetion(Attack, fTimeDelta, 5);
    m_skelAnime->Update_Animetion(Attack, fTimeDelta, 6);
}

void CWarden::Motion_Dead(_float fTimeDelta)
{
    m_skelAnime->Update_Animetion(Dead, fTimeDelta, 0);

    if (m_skelAnime->is_AnimtionEND(Dead))
    {
        //m_isDestroyed = true;
        m_pGameInstance->PlaySound(TEXT("Zombie_Death"), m_sound, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        m_pGameInstance->PopPool(this, TEXT("Layer_Monster"));
    }
}

void CWarden::Turn(_float fTimeDelta)
{
}

HRESULT CWarden::Ready_BehaviorTree()
{
    // 루트 노드: Selector (적을 발견하면 따라가고, 아니면 순찰)
    CSelectorNode* pRoot = new CSelectorNode(L"Root");

    // 조건 검사 노드: 적이 있는지 확인
    CBTTask_DetectEnemy* pDetectEnemy = new CBTTask_DetectEnemy;

    // 행동노드
    CBTTask_Chase* pChase = new CBTTask_Chase;
    CBTTask_Patrol* pPatrol = new CBTTask_Patrol;
    CBTTask_Attack* pAttack = new CBTTask_Attack;

    // 공격, 추격,
    CBTDistanceBranch* pDistanceBranch = new CBTDistanceBranch;
    pDistanceBranch->Set_Actions(pAttack, pChase, m_fAttackDistance);

    // 시퀀스
    CSequenceNode* pChaseSequence = new CSequenceNode(L"ChaseSequence");
    pChaseSequence->Add_Node(pDetectEnemy);
    pChaseSequence->Add_Node(pDistanceBranch);

    // 루트 노드에 추가
    pRoot->Add_Node(pChaseSequence);
    pRoot->Add_Node(pPatrol);

    // 최종 트리 설정
    m_pBehaviorTree = pRoot;

    return S_OK;
}

CWarden* CWarden::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CWarden* pInstance = new CWarden(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CWarden");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWarden::Clone(void* pArg)
{
    CWarden* pInstance = new CWarden(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CWarden");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWarden::FrameCallback(int animType, int frame)
{
}

void CWarden::Free()
{
    __super::Free();
}
