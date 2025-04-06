#include "BreakableCube.h"
#include "MCTerrain.h"
#include "GameInstance.h"
#include "Creeper.h"
#include "UI_Mgr.h"
#include <iostream>

CBreakableCube::CBreakableCube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CCube(pGraphic_Device) 
{
}

CBreakableCube::CBreakableCube(const CBreakableCube& Prototype)
    : CCube(Prototype)
{
}

HRESULT CBreakableCube::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CBreakableCube::Initialize(void* pArg)
{
    return S_OK;
}

void CBreakableCube::Priority_Update(_float fTimeDelta)
{

    //list<CGameObject*> Monsters{ m_pGameInstance->Get_GameObjectList(LEVEL_YU, TEXT("Layer_Monster")) };

    //for (CGameObject* pMonster : Monsters)
    //{
    //    if (CCreeper* _creeper = dynamic_cast<CCreeper*>(pMonster)) {
    //        _float3 creeperPos = _creeper->Get_Transform()->Get_State(CTransform::STATE_POSITION);

    //        for (CCollider_Cube* pCollider : m_Colliders)
    //        {
    //            _float3 vColliderPos{ m_pTransformCom->Get_State(CTransform::STATE_POSITION) + pCollider->Get_Offset() };

    //            _float3 vDiff{ creeperPos - vColliderPos };
    //            //vDiff.y *= 2.f; //y축으로는 충돌 계산 적게 하기위해
    //            _float fLengthSq{ D3DXVec3LengthSq(&vDiff) };

    //            if (fLengthSq < 7.f) {
    //                if (CPawn* _pawn = dynamic_cast<CPawn*>(_creeper)) {
    //                    if (CPawn::BOOM == _pawn->Get_ANIM()) {
    //                        Delete_Cube(vColliderPos);
    //                        m_pGameInstance->PopPool(pMonster);
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    if (m_fHp < 100) {

        m_resetHpFrame++;
        if (m_resetHpFrame > 10) {
            m_resetHpFrame = 0;
            m_fHp = 100;
            cout << "Reset Hp" << m_fHp << endl;

            m_pGameInstance->CheckSoundStop(this, 0, 1);
        }
    }

    if (m_fHp <= 0) {
        Delete_Cube(m_attackedBlockPos);
        m_fHp = 100;
    }

    if (m_vecPositions.size() == 0) {
        Destroy();
    }

    for (CCollider_Cube* pCollider : m_Colliders)
    {
        pCollider->Set_bColliderActive(false);
    }

}

void CBreakableCube::Update(_float fTimeDelta)
{
    Set_Bright();
}

void CBreakableCube::Late_Update(_float fTimeDelta)
{
    m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions, m_vecBrights);
}

HRESULT CBreakableCube::Render()
{
    if (FAILED(m_pTextureCom->Bind_Resource(0)))
        return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_Resource()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    m_pTransformCom->Bind_Resource(m_pShaderCom);
    m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);

    m_pShaderCom->Begin(0);

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pShaderCom->End();

    for (int i = 0; i < m_Colliders.size(); ++i)
    {
        if (m_Colliders[i]->Get_bColliderActive())
        {
            m_Colliders[i]->Render_Collider(true);

        }
    }

    return S_OK;
}


void CBreakableCube::Set_BlockPositions(vector<_float3> position, ITEMNAME _name)
{
    m_Colliders.clear();
    m_Colliders.resize(position.size());
    m_itemName = _name;

    for (int i = 0; i < position.size(); ++i) {
        m_vecPositions.push_back(position[i]); //위치 넣어줌
        m_vecBrights.push_back(0.6f);

        /* For.Com_Collider */
        CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
        Desc.vRadius = { .5f, .5f, .5f };
        Desc.vOffset = { position[i].x , position[i].y, position[i].z };
        Desc.pTransformCom = m_pTransformCom;
        Desc.pOwner = this;
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
            TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_Colliders[i]), &Desc)))
        {

        }

    }
}

HRESULT CBreakableCube::Delete_Cube(_float3 fPos)
{
    m_pGameInstance->CheckSoundStop(this, 0, 1);
    m_pGameInstance->PlaySound(TEXT("Block_BreakingFinish"), 1, fPos);
    return S_OK;
}

void CBreakableCube::Attacked_Block(_float3 fPos, int attackDamage)
{
    if (m_attackedBlockPos != fPos) {
        m_fHp = 100;
        cout << "Change Block" << m_fHp << endl;
    }

    if (m_fHp == 100) {
        m_pGameInstance->PlaySound(TEXT("Block_Breaking"), 1, fPos, this,1);
    }
    
    ITEMNAME _itemname = CUI_Mgr::Get_Instance()->GetItemTypeName();

    switch (_itemname)
    {
    case Client::ITEMNAME_PICKAXE:
        switch (m_itemName)
        {
        case Client::ITEMNAME_GRASSDIRT:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_DIRT:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_LEAF:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_WOOD:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_STONE:
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        case Client::ITEMNAME_COBBLESTONE:
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        case Client::ITEMNAME_COALORE:
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        case Client::ITEMNAME_IRONORE:
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        default:
            break;
        }
    default:
        switch (m_itemName)
        {
        case Client::ITEMNAME_GRASSDIRT:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_DIRT:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_LEAF:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_WOOD:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_STONE:
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_COBBLESTONE:
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_COALORE:
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_IRONORE:
            m_fHp -= attackDamage / 5.f;
            break;
        default:
            break;
        }

        break;
    }
    m_attackedBlockPos = fPos;
    m_resetHpFrame = 0;


    CParticleEventManager::Get_Instance()->OnParticle(
        PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_MINING,
        fPos);
}

float CBreakableCube::GetHP() const
{
    return m_fHp;
}

void CBreakableCube::Set_Bright()
{

    if (m_bChunkColliderActive)
    {
        CGameObject* pSteve{ nullptr };
        pSteve = m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve"));

        CTransform* pTransformCom{ nullptr };
        pTransformCom = static_cast<CTransform*>(pSteve->Find_Component(TEXT("Com_Transform")));
        _float3 vStevePos = { pTransformCom->Get_State(CTransform::STATE_POSITION) };


        for (int i = 0; i < m_vecPositions.size(); ++i) {
            _float3 vDiff{ vStevePos - m_vecPositions[i] };
            _float fLengthSq{ D3DXVec3LengthSq(&vDiff) };

            if (fLengthSq < 5.f) {
                m_vecBrights[i] = g_fBright + 0.2f * m_vecPositions[i].y / 10.f;
            }
            else if (fLengthSq < 10.f) {
                m_vecBrights[i] = g_fBright + 0.1f * m_vecPositions[i].y / 10.f;
            }
            else {
                m_vecBrights[i] = g_fBright * m_vecPositions[i].y / 10.f;
            }
        }
    }
    else {
        for (int i = 0; i < m_vecBrights.size(); ++i) {
            m_vecBrights[i] = g_fBright * m_vecPositions[i].y / 10.f;
        }
    }
}

HRESULT CBreakableCube::Create_Cube(_float3 fPos, _float3 _Dir)
{
    int brightIndex = 0;
    // 2. 벡터에서 해당 위치 추가
    for (int i = 0; i < m_vecPositions.size(); ++i) {
        if (fPos.x == m_vecPositions[i].x && fPos.y == m_vecPositions[i].y && fPos.z == m_vecPositions[i].z) {
            brightIndex = i;
            break;
        }
    }

    _float3 blockPos = fPos + _Dir;
    m_vecPositions.push_back(blockPos);
    m_vecBrights.push_back(m_vecBrights[brightIndex]);

    // 3. 콜라이더 추가
    /* For.Com_Collider */
    CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
    Desc.vRadius = { .5f, .5f, .5f };
    Desc.vOffset = { blockPos.x , blockPos.y, blockPos.z };
    Desc.pTransformCom = m_pTransformCom;
    Desc.pOwner = this;
    m_Colliders.resize(m_Colliders.size() + 1);
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&m_Colliders.back()), &Desc)))
    {
        return E_FAIL;
    }

    // 4. 인스턴스 버퍼 업데이트
    m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions, m_vecBrights);

    return S_OK;
}





HRESULT CBreakableCube::Ready_Components()
{
    /* For.Com_VIBuffer */
    Engine::CUBE cube{ _float2(64.f, 32.f), _float3(16.f, 16.f, 16.f), _float2(0.f, 0.f) };
    m_pVIBufferCom = CVIBuffer_CubeInstance::Create(m_pGraphic_Device, cube);
    //if (FAILED(__super::Add_Component(LEVEL_YU, TEXT("Prototype_Component_VIBuffer_CubeInstance"),
    //    TEXT("m_pVIBufferCom_CubeInstance"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
    //    return E_FAIL;

    /* For.Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 10.f, D3DXToRadian(90.f) };
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_CubeInstance"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    return S_OK;
}


void CBreakableCube::Should_Collide_With_Player()
{
    // 플레이어 밑에 있는 청크면 충돌 매니저에 올림(이제는 플레이어에다가 추가로 몬스터 크리퍼, 좀비 레이어)

    _float3 vStevePos; 

    if (CPawn* _steve = dynamic_cast<CPawn*>(m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve")))) {
        vStevePos = _steve->Get_Transform()->Get_State(CTransform::STATE_POSITION) + _float3{ 0.f, 1.5f, 0.f };
    }

    //플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
    for (int i = 0; i < m_vecPositions.size(); ++i) {
        _float3 vDiff{ vStevePos - m_vecPositions[i]};
        _float fLengthSq{ D3DXVec3LengthSq(&vDiff) };
        if (fLengthSq < 30.f)
        {
            //플레이어와 거리가 가까우면
            m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, m_Colliders[i]);

            m_Colliders[i]->Set_bColliderActive(true);
        }
    }
}

void CBreakableCube::Should_Collide_With_Monster()
{
    // 플레이어 밑에 있는 청크면 충돌 매니저에 올림(이제는 플레이어에다가 추가로 몬스터 크리퍼, 좀비 레이어)
    list<CGameObject*> Monsters{ m_pGameInstance->Get_GameObjectList(LEVEL_YU, TEXT("Layer_Monster")) };

    for (CGameObject* pMonster : Monsters)
    {

        /*_float3 pMonsterPos = static_cast<CPawn*>(pMonster)->Get_Transform()->Get_State(CTransform::STATE_POSITION);
        if (!m_pGameInstance->Is_In_Frustum(pMonsterPos, 0.5f))
            continue;*/

        CTransform* pTransformCom{ nullptr };
        pTransformCom = static_cast<CTransform*>(pMonster->Find_Component(TEXT("Com_Transform")));

        /*if (!m_pGameInstance->Is_In_Frustum(pTransformCom->Get_State(CTransform::STATE_POSITION), 0.5f))
            continue;*/

        _float3 _monPos;
        if (CPawn* _monster = dynamic_cast<CPawn*>(pMonster)) {
            _monPos = _monster->Get_Transform()->Get_State(CTransform::STATE_POSITION);
        }
        //if (!m_pGameInstance->Is_In_Frustum(_monPos, 0.5f))
        //    continue;


        _monPos += _float3{ 0.f, 1.f, 0.f };

        //플레이어와 가까이 있는 콜라이더만 활성화 시키고 등록함
        for (int i = 0; i < m_vecPositions.size(); ++i) {
            _float3 vDiff{ _monPos - m_vecPositions[i]};
            _float fLengthSq{ D3DXVec3LengthSq(&vDiff) };
            if (fLengthSq < /*0.f*/5.f)
            {
                //플레이어와 거리가 가까우면
                m_pGameInstance->Add_Collider_CollisionGroup(COLLISION_BLOCK, m_Colliders[i]);

                m_Colliders[i]->Set_bColliderActive(true);
            }
        }
    } 
}

CBreakableCube* CBreakableCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CBreakableCube* pInstance = new CBreakableCube(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBreakableCube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBreakableCube::Clone(void* pArg)
{
    CBreakableCube* pInstance = new CBreakableCube(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CBreakableCube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBreakableCube::Free()
{
    //if (CMCTerrain* _copy = dynamic_cast<CMCTerrain*>(m_pGameInstance->Get_Object(LEVEL_YU, TEXT("Layer_Terrain"), 0))){
    //   _copy->CheckColliderActive();
    //}

    __super::Free();
    Safe_Release(m_pVIBufferCom);
    //Safe_Release(m_pColliderCom);

	for (CCollider_Cube* m_Colliders : m_Colliders)
		Safe_Release(m_Colliders);

    Safe_Release(m_pShaderCom);
}
