#include "Creeper.h"

#include "Transform.h"
#include "Texture.h"
#include "VIBuffer_Cube.h"
#include "GameInstance.h"

CCreeper::CCreeper(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CMonster{ pGraphic_Device }
{
    for (int i = 0; i < 6; i++)
    {
       m_pVIBufferCom[i] = nullptr;     
    }
}

CCreeper::CCreeper(const CCreeper& Prototype)
    : CMonster(Prototype)
{
    for (int i = 0; i < 6; i++)
    {
        m_pVIBufferCom[i] = nullptr;
    }
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

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, { -10.f, 0.f, -10.f });

    return S_OK;
}

void CCreeper::Priority_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_CollisionGroup(COLLISION_MONSTER, this);
}

void CCreeper::Update(_float fTimeDelta)
{ 
    __super::Update(fTimeDelta);

    // 걷는 모션
    if (GetKeyState('Q') & 0x8000)
    {
        elapsedTime += fTimeDelta;
        Comput = maxAngle * sin(elapsedTime * D3DXToRadian(2.5f));

        //vecBones[3].transform.Turn_Radian(_float3(1.f, 0.f, 0.f), Comput);
        //vecBones[4].transform.Turn_Radian(_float3(1.f, 0.f, 0.f), -Comput);
        //vecBones[5].transform.Turn_Radian(_float3(1.f, 0.f, 0.f), -Comput);
        //vecBones[6].transform.Turn_Radian(_float3(1.f, 0.f, 0.f), Comput);
    }

    m_pCollider_CubeCom->Update_ColliderBox();

    CGameObject* pHitObject{ nullptr };
    _float fDist{};

    pHitObject = m_pGameInstance->Ray_Cast
    (   m_pTransformCom->Get_State(CTransform::STATE_POSITION),
        m_pTransformCom->Get_State(CTransform::STATE_LOOK),
        10.f,
        COLLISION_PLAYER,
        fDist
    );

}

void CCreeper::Late_Update(_float fTimeDelta)
{
 
    // 본의 매트릭스를 현재 트랜스폼으로 업데이트
    //vecBones[0].transform = *(m_pTransformCom->Get_WorldMatrix());

    // 본이 적용된 매쉬 업데이트
    Ready_Mesh();

    if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this)))
        return;
}

HRESULT CCreeper::Render()
{

    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    /*if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;*/
    for (int i = 0; i < 6; i++)
    {
        if (FAILED(m_pVIBufferCom[i]->Bind_WorldMatrix()))
            return E_FAIL;

        if (FAILED(m_pVIBufferCom[i]->Bind_Buffers()))
            return E_FAIL;

        if (FAILED(m_pVIBufferCom[i]->Render()))
            return E_FAIL;
    }

    m_pCollider_CubeCom->Render_ColliderBox(false);

    return S_OK;
}

HRESULT CCreeper::Ready_Components()
{

#pragma region 외형
    // 크리퍼 텍스처
   /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Creeper"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Creeper_Head"),
        TEXT("m_pVIBufferCom_Head"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[0]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Creeper_Body"),
        TEXT("m_pVIBufferCom_Body"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[1]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
        TEXT("m_pVIBufferCom_Foot_LF"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[2]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
        TEXT("m_pVIBufferCom_Foot_RF"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[3]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
        TEXT("m_pVIBufferCom_Foot_LB"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[4]))))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Creeper_Foot"),
        TEXT("m_pVIBufferCom_Foot_RB"), reinterpret_cast<CComponent**>(&m_pVIBufferCom[5]))))
        return E_FAIL;


    if (FAILED(Ready_Bone()))
        return E_FAIL;
    if (FAILED(Ready_Mesh()))
        return E_FAIL;
#pragma endregion

#pragma region 기능
    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 3.f, D3DXToRadian(30.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* For.Com_Collider_Cube */
    CCollider_Cube::COLLCUBE_DESC		ColliderCubeDesc;
    ColliderCubeDesc.fRadiusY = 1.f;
    ColliderCubeDesc.fOffSetY = 1.f;
    ColliderCubeDesc.pTransformCom = m_pTransformCom;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_pCollider_CubeCom), &ColliderCubeDesc)))
        return E_FAIL;
#pragma endregion

    

    return S_OK;
}

HRESULT CCreeper::Ready_Bone()
{

    /*BONE bone[7] = 
    {
         { "Root"    ,0, -1,  MAtrixTranslation(0.f,              0.f,      0.f) },
         { "Pelvis"  ,1,  0,  MAtrixTranslation(0,              6.f /16.f,  0) },
         { "Neck"    ,2,  1,  MAtrixTranslation(0,              12.f/16.f,  0) },
         { "Leg_LF"  ,3,  1,  MAtrixTranslation(2.f/16.f,       0,          -2.f/16.f) },
         { "Leg_RF"  ,4,  1,  MAtrixTranslation(-2.f/16.f,      0,          -2.f/16.f) },
         { "Leg_LB"  ,5,  1,  MAtrixTranslation(2.f/16.f,       0,          2.f/16.f) },
         { "Leg_RB"  ,6,  1,  MAtrixTranslation(-2.f/16,        0,          2.f/16.f) },
    };

    for (int i = 0; i < 7; i++)
    {
        vecBones.push_back(bone[i]);
    }*/

    return S_OK;
}

HRESULT CCreeper::Ready_Mesh()
{
    //Matrix temp = {};
    //Matrix rootTransform = vecBones[1].transform * vecBones[0].transform;

    //// 머리
    //temp = MAtrixTranslation(0, 4.f / 16.f, 0.f) *  vecBones[2].transform * rootTransform;
    //m_pVIBufferCom[0]->SetMatrix(temp);

    //// 몸통
    //temp = MAtrixTranslation(0, 6.f / 16.f, 0.f) * rootTransform;
    //m_pVIBufferCom[1]->SetMatrix(temp);

    //// 다리
    //const float legOffsets[4][2] = {
    //   {-3.f / 16.f, -2.f / 16.f},
    //   {-3.f / 16.f, -2.f / 16.f},
    //   {-3.f / 16.f,  2.f / 16.f},
    //   {-3.f / 16.f,  2.f / 16.f}
    //};

    //for (int i = 0; i < 4; ++i)
    //{
    //    temp = MAtrixTranslation(0, legOffsets[i][0], legOffsets[i][1]) * vecBones[i + 3].transform * rootTransform;
    //    m_pVIBufferCom[i + 2]->SetMatrix(temp);
    //}

    return S_OK;
}

HRESULT CCreeper::UpDate_Mesh()
{



    return S_OK;
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

    Safe_Release(m_pCollider_CubeCom);
    Safe_Release(m_pTextureCom);

    for (int i = 0; i < 6; i++)
    {
        Safe_Release(m_pVIBufferCom[i]);
    }
  
}
