#include "Warden.h"

#include "BTTask_WalkChase.h"
#include "BTTask_Idle.h"
#include "BTDecorator_IsAttack.h"
#include "BTTask_Find.h"
#include "BTTask_RunChase.h"
#include "BTTask_Attack_Near.h"
#include "BTTask_Attack_Far.h"
#include "BTDecorator_IsCutScene.h"

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
    m_fSpeed = 2.5f;
    m_Hp = 500.f; 
    m_MaxHp = 500.f; 

    m_Coll_Size = { 0.5f, 1.0f, 0.5f };
    m_Coll_Offset = { 0.f, 0.6f, 0.f };

    __super::Initialize(pArg);

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Bone()))
        return E_FAIL;

    if (FAILED(Ready_Animation()))
        return E_FAIL;

    // 콜백 등록
    m_skelAnime->SetFrameCallback(std::bind(&CWarden::FrameCallback, this, std::placeholders::_1, std::placeholders::_2));

    // 임시 스폰
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,_float3(10, 15, 10));

    SetAnimTime();
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

    //m_pGameInstance->Check_Sound_Stop(this, m_eCurAnim, 0);
}

void CWarden::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
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

void CWarden::Dead_Pawn()
{
    CPawn::Dead_Pawn();

    m_pGameInstance->Play_Sound(TEXT("death_1"), SOUND_DEAD, this, m_sound, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

}

void CWarden::SetAnimTime()
{
    m_skelAnime->Set_fElapsedTime(Rest_Pelvis, 2.f);
    m_skelAnime->Set_fElapsedTime(Rest_Arm_R, 2.f);
    m_skelAnime->Set_fElapsedTime(Rest_Arm_L, 2.f);
    m_skelAnime->Set_fElapsedTime(Rest_Neck, 2.f);
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
    * Run 모션
    ------------*/
    // Run_Pevis, Run_Leg_R, Run_Leg_L, Run_Arm_R, Run_Arm_L,

    Pelvis =  { 0.f, mat }; //0
    Pelvis1 = { 0.5, mat2 }; // 25
    Pelvis2 = { 1, mat6 }; // 0
    Pelvis3 = { 1.5, mat3 }; // -25
    Pelvis4 = { 2, mat }; // 0

    Leg_R1 = { 0.f, mat }; //0
    Leg_R2 = { 0.25f, mat4 }; // 
    Leg_R3 = { 0.5, mat }; // 0
    Leg_R4 = { 0.75f, mat5 }; // 
    Leg_R5 = { 1.f, mat }; // 0
    Leg_R6 = { 1.25f, mat4 }; // 0
    Leg_R7 = { 1.5f, mat }; // 0
    Leg_R8 = { 1.75f, mat5 }; // 0
    Leg_R9 = { 2.f, mat }; // 0

    Leg_L1 = { 0.f, mat };
    Leg_L2 = { 0.25f, mat5 };
    Leg_L3 = { 0.5f, mat };
    Leg_L4 = { 0.75f, mat4 };
    Leg_L5 = { 1.f, mat };
    Leg_L6 = { 1.25f, mat5 };
    Leg_L7 = { 1.5f, mat };
    Leg_L8 = { 1.75f, mat4 };
    Leg_L9 = { 2.f, mat };

    m_skelAnime->Add_Animation(ANIM_type::Run_Pevis, Pelvis);
    m_skelAnime->Add_Animation(ANIM_type::Run_Pevis, Pelvis1);
    m_skelAnime->Add_Animation(ANIM_type::Run_Pevis, Pelvis2);
    m_skelAnime->Add_Animation(ANIM_type::Run_Pevis, Pelvis3);
    m_skelAnime->Add_Animation(ANIM_type::Run_Pevis, Pelvis4);
                                          
    m_skelAnime->Add_Animation(ANIM_type::Run_Leg_R, Leg_R1);
    m_skelAnime->Add_Animation(ANIM_type::Run_Leg_R, Leg_R2);
    m_skelAnime->Add_Animation(ANIM_type::Run_Leg_R, Leg_R3);
    m_skelAnime->Add_Animation(ANIM_type::Run_Leg_R, Leg_R4);
    m_skelAnime->Add_Animation(ANIM_type::Run_Leg_R, Leg_R5);
                                          
    m_skelAnime->Add_Animation(ANIM_type::Run_Leg_L, Leg_L1);
    m_skelAnime->Add_Animation(ANIM_type::Run_Leg_L, Leg_L2);
    m_skelAnime->Add_Animation(ANIM_type::Run_Leg_L, Leg_L3);
    m_skelAnime->Add_Animation(ANIM_type::Run_Leg_L, Leg_L4);
    m_skelAnime->Add_Animation(ANIM_type::Run_Leg_L, Leg_L5);
                                          
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_R, Leg_L1);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_R, Leg_L2);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_R, Leg_L3);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_R, Leg_L4);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_R, Leg_L5);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_R, Leg_L6);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_R, Leg_L7);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_R, Leg_L8);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_R, Leg_L9);
                                          
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_L, Leg_R1);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_L, Leg_R2);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_L, Leg_R3);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_L, Leg_R4);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_L, Leg_R5);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_L, Leg_R6);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_L, Leg_R7);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_L, Leg_R8);
    m_skelAnime->Add_Animation(ANIM_type::Run_Arm_L, Leg_R9);

    /*----------
    * Attack 모션
    ------------*/

    mat2 = {};
    mat2.Turn_Radian(_float3(1.f, -0.1f, -0.1f), D3DXToRadian(-170));

    mat3 = {};
    mat3.Turn_Radian(_float3(1.f, 4.f, 1.5f), D3DXToRadian(-40));

    mat4 = {};
    mat4.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(50));

    Matrix mat7 = {};
    mat7.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-30));

    mat5 = {};
    mat5.Turn_Radian(_float3(1.f, 0.1f, 0.1f), D3DXToRadian(-170));

    mat6 = {};
    mat6.Turn_Radian(_float3(1.f, -4.f, -1.5f), D3DXToRadian(-40));

    KEYFREAME Attack_R1 = { 0.f, mat }; 
    KEYFREAME Attack_R2 = { 0.4f, mat2 };
    KEYFREAME Attack_R3 = { 0.6f, mat3 };
    KEYFREAME Attack_R4 = { 0.61f, mat };

    KEYFREAME Attack_P1 = { 0.f, mat }; 
    KEYFREAME Attack_P2 = { 0.4f, mat7 }; 
    KEYFREAME Attack_P3 = { 0.6f, mat4 };
    KEYFREAME Attack_P4 = { 0.61f, mat };

    KEYFREAME Attack_L1 = { 0.f, mat }; 
    KEYFREAME Attack_L2 = { 0.4f, mat5 };
    KEYFREAME Attack_L3 = { 0.6f, mat6 };
    KEYFREAME Attack_L4 = { 0.61f, mat };

    m_skelAnime->Add_Animation(ANIM_type::Attack_Arm_R, Attack_R1);
    m_skelAnime->Add_Animation(ANIM_type::Attack_Arm_R, Attack_R2);
    m_skelAnime->Add_Animation(ANIM_type::Attack_Arm_R, Attack_R3);
    m_skelAnime->Add_Animation(ANIM_type::Attack_Arm_R, Attack_R4);

    m_skelAnime->Add_Animation(ANIM_type::Attack_Pevis, Attack_P1);
    m_skelAnime->Add_Animation(ANIM_type::Attack_Pevis, Attack_P2);
    m_skelAnime->Add_Animation(ANIM_type::Attack_Pevis, Attack_P3);
    m_skelAnime->Add_Animation(ANIM_type::Attack_Pevis, Attack_P4);

    m_skelAnime->Add_Animation(ANIM_type::Attack_Arm_L, Attack_L1);
    m_skelAnime->Add_Animation(ANIM_type::Attack_Arm_L, Attack_L2);
    m_skelAnime->Add_Animation(ANIM_type::Attack_Arm_L, Attack_L3);
    m_skelAnime->Add_Animation(ANIM_type::Attack_Arm_L, Attack_L4);

    /*----------
     Find 모션
    --------*/
    mat2 = {};
    mat2.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-30));

    mat3 = {};
    mat3.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(70));

    mat4 = {};
    mat4.Turn_Radian(_float3(1.f, -0.5f, -0.05f), D3DXToRadian(70));

    mat5 = {};
    mat5.Turn_Radian(_float3(1.f, 0.5f, 0.05f), D3DXToRadian(70));

    mat6 = {};
    mat6.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(20));

    mat7 = {};
    mat7.Turn_Radian(_float3(0.f, 0.f, 1.f), D3DXToRadian(-20));

    Matrix mat8 = {};
    mat8.Turn_Radian(_float3(1.f, 0.3f, -0.1f), D3DXToRadian(-110));

    Matrix mat9 = {};
    mat9.Turn_Radian(_float3(1.f, -0.3f, 0.1f), D3DXToRadian(-110));

    KEYFREAME Find_P1 = { 0.f, mat };
    KEYFREAME Find_P2 = { 0.5f, mat2 };
    KEYFREAME Find_P3 = { 1.2f, mat3 };
    KEYFREAME Find_P4 = { 2.f,  mat3 };

    KEYFREAME Find_Arm_R1 = { 0.f, mat };
    KEYFREAME Find_Arm_R2 = { 0.8f, mat8 };
    KEYFREAME Find_Arm_R3 = { 1.2f, mat4 };
    KEYFREAME Find_Arm_R4 = { 2.f, mat4 };

    KEYFREAME Find_Arm_L1 = { 0.f, mat };
    KEYFREAME Find_Arm_L2 = { 0.8f, mat9 };
    KEYFREAME Find_Arm_L3 = { 1.2f, mat5 };
    KEYFREAME Find_Arm_L4 = { 2.f, mat5 };

    KEYFREAME Find_Neck1 = { 0.f, mat };
    KEYFREAME Find_Neck2 = { 0.9f, mat };
    KEYFREAME Find_Neck3 = { 1.2f, mat7 };
    KEYFREAME Find_Neck4 = { 1.5f, mat6 };
    KEYFREAME Find_Neck5 = { 1.8f, mat7 };
    KEYFREAME Find_Neck6 = { 2.f, mat };

    m_skelAnime->Add_Animation(ANIM_type::Find_Pevis, Find_P1);
    m_skelAnime->Add_Animation(ANIM_type::Find_Pevis, Find_P2);
    m_skelAnime->Add_Animation(ANIM_type::Find_Pevis, Find_P3);
    m_skelAnime->Add_Animation(ANIM_type::Find_Pevis, Find_P4);

    m_skelAnime->Add_Animation(ANIM_type::Find_Arm_R, Find_Arm_R1);
    m_skelAnime->Add_Animation(ANIM_type::Find_Arm_R, Find_Arm_R2);
    m_skelAnime->Add_Animation(ANIM_type::Find_Arm_R, Find_Arm_R3);
    m_skelAnime->Add_Animation(ANIM_type::Find_Arm_R, Find_Arm_R4);

    m_skelAnime->Add_Animation(ANIM_type::Find_Arm_L, Find_Arm_L1);
    m_skelAnime->Add_Animation(ANIM_type::Find_Arm_L, Find_Arm_L2);
    m_skelAnime->Add_Animation(ANIM_type::Find_Arm_L, Find_Arm_L3);
    m_skelAnime->Add_Animation(ANIM_type::Find_Arm_L, Find_Arm_L4);

    m_skelAnime->Add_Animation(ANIM_type::Find_Neck, Find_Neck1);
    m_skelAnime->Add_Animation(ANIM_type::Find_Neck, Find_Neck2);
    m_skelAnime->Add_Animation(ANIM_type::Find_Neck, Find_Neck3);
    m_skelAnime->Add_Animation(ANIM_type::Find_Neck, Find_Neck4);
    m_skelAnime->Add_Animation(ANIM_type::Find_Neck, Find_Neck5);
    m_skelAnime->Add_Animation(ANIM_type::Find_Neck, Find_Neck6);

   /*----------
    Attack_2 모션
    --------*/

    mat2 = {};
    mat2.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(70));

    mat6 = {};
    mat6.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(120));

    mat3 = {};
    mat3.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-30));

    mat4 = {};
    mat4.Turn_Radian(_float3(1.5f, -2.f, -0.5f), D3DXToRadian(-110));

    mat5 = {};
    mat5.Turn_Radian(_float3(1.5f, 2.f, 0.5f), D3DXToRadian(-110));

    mat7 = {};
    mat7.Turn_Radian(_float3(1.5f, -1.f, -1.f), D3DXToRadian(50));

    mat8 = {};
    mat8.Turn_Radian(_float3(1.5f, 1.f, 1.f), D3DXToRadian(50));

    KEYFREAME Attack2_Pelvis_1 = { 0.f, mat };
    KEYFREAME Attack2_Pelvis_2 = { 1.2f, mat2 };
    KEYFREAME Attack2_Pelvis_3 = { 1.5f,  mat3 };
    KEYFREAME Attack2_Pelvis_4 = { 2.f,  mat3 };

    KEYFREAME Attack2_Neck_1 = { 0.f, mat };
    KEYFREAME Attack2_Neck_2 = { 1.2f, mat6 };
    KEYFREAME Attack2_Neck_3 = { 1.5f,  mat3 };
    KEYFREAME Attack2_Neck_4 = { 2.f,  mat3 };

    KEYFREAME Attack2_ArmR_1 = { 0.f, mat };
    KEYFREAME Attack2_ArmR_2 = { 1.2f, mat4 };
    KEYFREAME Attack2_ArmR_3 = { 1.5f,  mat7 };
    KEYFREAME Attack2_ArmR_4 = { 2.f,  mat7 };

    KEYFREAME Attack2_ArmL_1 = { 0.f, mat };
    KEYFREAME Attack2_ArmL_2 = { 1.2f, mat5 };
    KEYFREAME Attack2_ArmL_3 = { 1.5f,  mat8 };
    KEYFREAME Attack2_ArmL_4 = { 2.f,  mat8 };

    m_skelAnime->Add_Animation(ANIM_type::Attack2_Pevis, Attack2_Pelvis_1);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Pevis, Attack2_Pelvis_2);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Pevis, Attack2_Pelvis_3);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Pevis, Attack2_Pelvis_4);

    m_skelAnime->Add_Animation(ANIM_type::Attack2_Neck, Attack2_Neck_1);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Neck, Attack2_Neck_2);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Neck, Attack2_Neck_3);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Neck, Attack2_Neck_4);

    m_skelAnime->Add_Animation(ANIM_type::Attack2_Arm_R, Attack2_ArmR_1);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Arm_R, Attack2_ArmR_2);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Arm_R, Attack2_ArmR_3);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Arm_R, Attack2_ArmR_4);

    m_skelAnime->Add_Animation(ANIM_type::Attack2_Arm_L, Attack2_ArmL_1);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Arm_L, Attack2_ArmL_2);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Arm_L, Attack2_ArmL_3);
    m_skelAnime->Add_Animation(ANIM_type::Attack2_Arm_L, Attack2_ArmL_4);

/*----------
* Rest 모션
------------*/
    mat2 = {};
    mat2.Turn_Radian(_float3(1.f, 0.5f, 0.f), D3DXToRadian(80));

    mat3 = {};
    mat3.Turn_Radian(_float3(-2.5f, 1.f, 0.f), D3DXToRadian(-30));

    mat4 = {};
    mat4.Turn_Radian(_float3(1.f, 0.f, 0.f), D3DXToRadian(-60));

    mat5 = {};
    mat5.Turn_Radian(_float3(1.f, 1.f, 0.f), D3DXToRadian(-80));

    KEYFREAME Rest_Pelvis_1 = { 0.f, mat };
    KEYFREAME Rest_Pelvis_2 = { 1.5f, mat2 };
    KEYFREAME Rest_Pelvis_3 = { 3.5f, mat2 };
    KEYFREAME Rest_Pelvis_4 = { 5.f, mat };

    KEYFREAME Rest_Neck_1 = { 0.f, mat };
    KEYFREAME Rest_Neck_2 = { 1.5f, mat3 };
    KEYFREAME Rest_Neck_3 = { 3.5f, mat3 };
    KEYFREAME Rest_Neck_4 = { 5.f, mat };

    KEYFREAME Rest_ArmR_1 = { 0.f, mat };
    KEYFREAME Rest_ArmR_2 = { 1.5f, mat4 };
    KEYFREAME Rest_ArmR_3 = { 3.5f, mat4 };
    KEYFREAME Rest_ArmR_4 = { 5.f, mat };

    KEYFREAME Rest_ArmL_1 = { 0.f, mat };
    KEYFREAME Rest_ArmL_2 = { 1.5f, mat5 };
    KEYFREAME Rest_ArmL_3 = { 3.5f, mat5 };
    KEYFREAME Rest_ArmL_4 = { 5.f, mat };


    m_skelAnime->Add_Animation(ANIM_type::Rest_Pelvis, Rest_Pelvis_1);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Pelvis, Rest_Pelvis_2);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Pelvis, Rest_Pelvis_3);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Pelvis, Rest_Pelvis_4);

    m_skelAnime->Add_Animation(ANIM_type::Rest_Neck, Rest_Neck_1);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Neck, Rest_Neck_2);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Neck, Rest_Neck_3);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Neck, Rest_Neck_4);

    m_skelAnime->Add_Animation(ANIM_type::Rest_Arm_R, Rest_ArmR_1);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Arm_R, Rest_ArmR_2);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Arm_R, Rest_ArmR_3);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Arm_R, Rest_ArmR_4);

    m_skelAnime->Add_Animation(ANIM_type::Rest_Arm_L, Rest_ArmL_1);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Arm_L, Rest_ArmL_2);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Arm_L, Rest_ArmL_3);
    m_skelAnime->Add_Animation(ANIM_type::Rest_Arm_L, Rest_ArmL_4);

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
    case  CWarden::ATTACK2:
        Motion_Attack2(fTimeDelta);
        break;
    case  CWarden::FIND:
        Motion_Find(fTimeDelta);
        break;
    case  CWarden::DEAD:
        Motion_Dead(fTimeDelta);
        break;
    case  CWarden::RUN:
        Motion_Run(fTimeDelta);
        break;
    case CWarden::ANIM_END:
        break;
    default:
        break;
    }
}

void CWarden::Motion_Idle(_float fTimeDelta)
{
    if (m_skelAnime->is_AnimtionEND(Rest_Pelvis) &&
        m_skelAnime->is_AnimtionEND(Rest_Neck) &&
        m_skelAnime->is_AnimtionEND(Rest_Arm_L) &&
        m_skelAnime->is_AnimtionEND(Rest_Arm_R)
        )
    {
        m_bAnimEnd[IDLE] = true;
    }

    m_skelAnime->Update_Animetion(Rest_Pelvis, fTimeDelta, 1);
    m_skelAnime->Update_Animetion(Rest_Neck, fTimeDelta, 2);
    m_skelAnime->Update_Animetion(Rest_Arm_R, fTimeDelta, 5);
    m_skelAnime->Update_Animetion(Rest_Arm_L, fTimeDelta, 6);
}

void CWarden::Motion_Walk(_float fTimeDelta)
{
    m_pTransformCom->Set_SpeedperSec(2.5f);
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
    m_fAttackDistance = 5.f;
    if (m_skelAnime->is_AnimtionEND(Attack_Arm_R) &&
        m_skelAnime->is_AnimtionEND(Attack_Arm_L) &&
        m_skelAnime->is_AnimtionEND(Attack_Pevis))
    {
        m_bAnimEnd[ATTACK] = true;
        _float3 temp = m_pTargetPawn->Get_Transform()->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
        m_pTargetPawn->Knock_back(temp);        
    }

    m_skelAnime->Update_Animetion(Attack_Pevis, fTimeDelta, 1);
    m_skelAnime->Update_Animetion(Attack_Arm_R, fTimeDelta, 5);
    m_skelAnime->Update_Animetion(Attack_Arm_L, fTimeDelta, 6);
}

void CWarden::Motion_Attack2(_float fTimeDelta)
{
    m_fAttackDistance = 10.f;
    if (m_skelAnime->is_AnimtionEND(Attack2_Neck) &&
        m_skelAnime->is_AnimtionEND(Attack2_Pevis) &&
        m_skelAnime->is_AnimtionEND(Attack2_Arm_L)&&
        m_skelAnime->is_AnimtionEND(Attack2_Arm_R)
        )
    {
        m_bAnimEnd[ATTACK2] = true;
        isShootFollow = true;        

    }

    m_skelAnime->Update_Animetion(Attack2_Pevis, fTimeDelta, 1);
    m_skelAnime->Update_Animetion(Attack2_Neck, fTimeDelta, 2);
    m_skelAnime->Update_Animetion(Attack2_Arm_L, fTimeDelta, 5);
    m_skelAnime->Update_Animetion(Attack2_Arm_R, fTimeDelta, 6);

    Turn(fTimeDelta);
}

void CWarden::Motion_Dead(_float fTimeDelta)
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
        //m_pGameInstance->Play_Sound(TEXT("Zombie_Death"), SOUND_DEAD, this, m_sound, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        m_pGameInstance->PopPool(this, TEXT("Layer_Monster"));
    }
}

void CWarden::Motion_Find(_float fTimeDelta)
{
    if (m_skelAnime->is_AnimtionEND(Find_Pevis) &&
        m_skelAnime->is_AnimtionEND(Find_Arm_R) &&
        m_skelAnime->is_AnimtionEND(Find_Neck) &&
        m_skelAnime->is_AnimtionEND(Find_Arm_L))
    {
        m_bAnimEnd[FIND] = true;
    }

    m_skelAnime->Update_Animetion(Find_Pevis, fTimeDelta, 1);
    m_skelAnime->Update_Animetion(Find_Neck, fTimeDelta, 2);
    m_skelAnime->Update_Animetion(Find_Arm_R, fTimeDelta, 5);
    m_skelAnime->Update_Animetion(Find_Arm_L, fTimeDelta, 6);

    // 여기다 안두면 애니메이션어 덛어씌워짐
    LookAtPlayer(fTimeDelta);
}

void CWarden::Motion_Run(_float fTimeDelta)
{
    m_pTransformCom->Set_SpeedperSec(5.f);
    if (m_skelAnime->is_AnimtionEND(Run_Leg_R) &&
        m_skelAnime->is_AnimtionEND(Run_Leg_L) &&
        m_skelAnime->is_AnimtionEND(Run_Pevis) &&
        m_skelAnime->is_AnimtionEND(Run_Arm_R) &&
        m_skelAnime->is_AnimtionEND(Run_Arm_L)
        )
    {
        m_eCurAnim = RUN;
    }

    m_skelAnime->Update_Animetion(Run_Pevis, fTimeDelta, 1);
    m_skelAnime->Update_Animetion(Run_Leg_R, fTimeDelta, 3);
    m_skelAnime->Update_Animetion(Run_Leg_L, fTimeDelta, 4);
                                  
    m_skelAnime->Update_Animetion(Run_Arm_R, fTimeDelta, 5);
    m_skelAnime->Update_Animetion(Run_Arm_L, fTimeDelta, 6);
}

void CWarden::Turn(_float fTimeDelta)
{
    if (isShootFollow)
    {
        m_ShootPos = m_TargetPos;
        m_ShootPos.y += 1.f;
    }
    
    m_pTransformCom->LookAt_XZ(m_ShootPos);
}

void CWarden::Attack_Boom()
{
    m_pGameInstance->Play_Sound(TEXT("sonic_boom1"), SOUND_ATTACK, this, 1.f, Get_Transform()->Get_State(CTransform::STATE_POSITION));

    CGameObject* pHitObject{ nullptr };

    pHitObject = m_pGameInstance->Ray_Cast(
        Get_Transform()->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f, 1.f, 0.f },
        Get_Transform()->Get_State(CTransform::STATE_LOOK),
        20.f,
        COLLISION_PLAYER
    );
    if (pHitObject)
    {
        static_cast<CPawn*>(pHitObject)->Add_Hp(-40.f);

        _float3 vForce{ Get_Target()->Get_Transform()->Get_State(CTransform::STATE_POSITION) -Get_Transform()->Get_State(CTransform::STATE_POSITION) };
        D3DXVec3Normalize(&vForce, &vForce);
        vForce *= 15.f;
        vForce.y += 15.f;

        static_cast<CPawn*>(pHitObject)->Knock_back(vForce);
    }
}

HRESULT CWarden::Ready_BehaviorTree()
{
    // 루트 노드: Selector (적을 발견하면 따라가고, 아니면 순찰)
    CSelectorNode* pRoot = new CSelectorNode(L"Root");
    CBTDecorator_IsAttack* pIsAttack = new CBTDecorator_IsAttack;
    CBTDecorator_IsCutScene* pIsCutScene = new CBTDecorator_IsCutScene;

    // 행동노드
    CBTTask_WalkChase* pChase = new CBTTask_WalkChase;
    CBTTask_Idle* pIdle = new CBTTask_Idle;
    CRandomSelector* RandomSelector = new CRandomSelector;

    CSequenceNode* pNearAttack = new CSequenceNode(L"Near_Attack_Sequence");
    CSequenceNode* pFarAttack = new CSequenceNode(L"Far_Attack_Sequence");
    CSequenceNode* pCutScene = new CSequenceNode(L"CutScene_Sequence");

    CBTTask_Find* pFind = new CBTTask_Find;
    CBTTask_RunChase* pRunChase = new CBTTask_RunChase;
    CBTTask_Attack_Near* pAttackNear = new CBTTask_Attack_Near;
    CBTTask_Attack_Far* pAttackFar = new CBTTask_Attack_Far;

    pNearAttack->Add_Node(pFind);
    pNearAttack->Add_Node(pRunChase);
    pNearAttack->Add_Node(pAttackNear);

    pFarAttack->Add_Node(pFind);
    pFarAttack->Add_Node(pAttackFar);

    pIsAttack->Set_DecoratorNodes(RandomSelector,nullptr);
    RandomSelector->Add_Node(pNearAttack);
    RandomSelector->Add_Node(pFarAttack);

    pIsCutScene->Set_DecoratorNodes(pCutScene, nullptr);
    pCutScene->Add_Node(pFind);

    // 루트 노드에 추가
    pRoot->Add_Node(pIsCutScene);
    pRoot->Add_Node(pChase);
    pRoot->Add_Node(pIsAttack);
    pRoot->Add_Node(pIdle);

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
    if (animType == CWarden::Attack2_Pevis &&
        frame == 1)
    {
        isShootFollow = false;

        CParticleEventManager::Get_Instance()->OnParticle(
            PROTOTYPE_GAMEOBJECT_PARTICLE_SONIC_BOOM,
            m_pTransformCom,
            1.f
        );

        Attack_Boom();
    }

    if (animType == Find_Pevis)
    {
        m_pGameInstance->Play_Sound(TEXT("roar_1"), SOUND_SAY3, this, 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
    }

    /*if (animType == Attack_Pevis)
    {
        m_pGameInstance->Play_Sound(TEXT("attack_impact_1"), SOUND_ATTACK, this, m_sound - 0.35, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
    }*/

    if (animType == Attack2_Pevis)
    {
        m_pGameInstance->Play_Sound(TEXT("sonic_charge1"), SOUND_ATTACK, this, 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
    }
    
    if (animType == Swing_Pevis || animType == Run_Pevis)
    {
        m_pGameInstance->Play_Sound(TEXT("step_1"), SOUND_WALK1, this, 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        int random = rand() % 10;
        if (random < 4) {
            switch (random)
            {
            case 0:
                m_pGameInstance->Play_Sound(TEXT("ambient_1"), SOUND_SAY1, this, 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                break;
            case 1:
                m_pGameInstance->Play_Sound(TEXT("heartbeat_1"), SOUND_SAY2, this, 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
                break;
            //case 2:
            //    m_pGameInstance->Play_Sound(TEXT("heartbeat_2"), SOUND_SAY3, this, 1.f, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
            //    break;
            default:
                break;
            }
        }
    }

}

void CWarden::Free()
{
    __super::Free();
}
