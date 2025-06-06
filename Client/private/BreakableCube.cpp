﻿#include "BreakableCube.h"
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

    if (m_fHp < 100) {

        m_resetHpFrame++;
        if (m_resetHpFrame > 10) {
            m_resetHpFrame = 0;
            m_fHp = 100;
           // cout << "Reset Hp" << m_fHp << endl;

            //m_pGameInstance->Check_Sound_Stop(this, 0, 1);
        }
    }

    if (m_fHp <= 0) {
        Delete_Cube(m_attackedBlockPos);
        m_fHp = 100;
    }

    if (m_vecPositions.size() == 0) {
        Destroy();
    }

    
    //콜라이더 끄는 디버깅 코드 렉 심함
  //  for (auto& pair : m_Colliders)
  //  {
		//if (!pair.second)
		//	continue;

  //      pair.second->Set_bColliderActive(false);
  //  }

}

void CBreakableCube::Update(_float fTimeDelta)
{

}

void CBreakableCube::Late_Update(_float fTimeDelta)
{
    Set_Bright();
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
    m_pTransformCom->Bind_Resource_Scan(m_pShaderCom, g_fScanRange);
    m_pTransformCom->Bind_Resource_WardenWave(m_pShaderCom, g_fWaveRange);
    m_pTransformCom->Bind_Resource_BlockType(m_pShaderCom, m_itemName);

    if (m_Warden == nullptr)
    {
        m_Warden = (CWarden*)m_pGameInstance->Get_LastObject(
            LEVEL_YU,
            TEXT("Layer_Monster")
            );
    }

    if (m_Warden != nullptr && !m_Warden->Get_isDestroy()) //씬 넘어갈때 오류나서 !m_Warden->Get_isDestroy() 추가했으요(영웅)
    {
        _float4 pos = (_float4)m_Warden->Get_Transform()->Get_State(CTransform::STATE_POSITION);
        m_pTransformCom->Bind_Resource_WardenPosition(m_pShaderCom, &pos);
    }    

    m_pTextureCom->Bind_Resource(m_pShaderCom, "g_Texture", 1);

    if (g_bIsScan == true)
    {        
        m_pShaderCom->Begin(2);
    }    
    else if (g_bIsWave == true)
    {
        m_pShaderCom->Begin(3);
    }
    else
    {
        m_pShaderCom->Begin(0);
    }

    /* 정점을 그린다. */
    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    m_pShaderCom->End();

    //for (auto& pair : m_Colliders)
    //{
    //    if (!pair.second)
    //        continue;

    //    if (pair.second->Get_bColliderActive())
    //    {
    //        pair.second->Render_Collider(true);
    //    }
    //}


    return S_OK;
}


void CBreakableCube::Set_BlockPositions(vector<_float3> position, ITEMNAME _name)
{
    m_Colliders.clear();
    m_itemName = _name;

    for (int i = 0; i < position.size(); ++i) {
        m_vecPositions.push_back(position[i]); // 위치 넣어줌
        m_vecBrights.push_back(0.6f);

        _int3 key{};
        key.x = static_cast<_int>(position[i].x);
        key.y = static_cast<_int>(position[i].y);
        key.z = static_cast<_int>(position[i].z);

        // 이미 키가 존재하면 콜라이더 생성하지 않음
        if (m_Colliders.find(key) != m_Colliders.end())
            continue;

        // 콜라이더 생성
        CCollider_Cube::COLLCUBE_DESC Desc{};
        Desc.vRadius = { 0.5f, 0.5f, 0.5f };
        Desc.vOffset = { position[i].x , position[i].y, position[i].z };
        Desc.pTransformCom = m_pTransformCom;
        Desc.pOwner = this;

        CComponent* pCom = nullptr;
        if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
            TEXT("Com_Collider_Cube"), &pCom, &Desc)))
        {
            MSG_BOX("Failed To Add_Collider_Cube");
            continue;
        }

        m_Colliders.emplace(key, static_cast<CCollider_Cube*>(pCom));
    }
}


HRESULT CBreakableCube::Create_Cube(_float3 vPos, _float3 _Dir)
{
    int brightIndex = 0;
    // 2. 벡터에서 해당 위치 추가
    for (int i = 0; i < m_vecPositions.size(); ++i) {
        if (vPos.x == m_vecPositions[i].x && vPos.y == m_vecPositions[i].y && vPos.z == m_vecPositions[i].z) {
            brightIndex = i;
            break;
        }
    }

    _float3 blockPos = vPos + _Dir;
    m_vecPositions.push_back(blockPos);
    m_vecBrights.push_back(m_vecBrights[brightIndex]);

    // 3. 콜라이더 추가
    /* For.Com_Collider */
    CCollider_Cube::COLLCUBE_DESC Desc{}; //콜라이더 크기 설정
    Desc.vRadius = { .5f, .5f, .5f };
    Desc.vOffset = { blockPos.x , blockPos.y, blockPos.z };
    Desc.pTransformCom = m_pTransformCom;
    Desc.pOwner = this;

    CCollider_Cube* pCollder{ nullptr };

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Cube"),
        TEXT("Com_Collider_Cube"), reinterpret_cast<CComponent**>(&pCollder), &Desc)))
    {
        return E_FAIL;
    }

    _int3 key{
    static_cast<_int>(blockPos.x),
    static_cast<_int>(blockPos.y),
    static_cast<_int>(blockPos.z)
    };

    m_Colliders.emplace(key, pCollder);

    // 4. 인스턴스 버퍼 업데이트
    m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions, m_vecBrights);

    Play_Create_Sound(vPos);

    return S_OK;
}

void CBreakableCube::Play_Create_Sound(_float3 vPos)
{
    m_pGameInstance->Play_Sound(TEXT("Stone_dig2"), SOUND_BLOCK_DIG, this, 0.8f, vPos);
}


HRESULT CBreakableCube::Delete_Cube(_float3 vPos)
{
    _int3 key{
    static_cast<_int>(vPos.x),
    static_cast<_int>(vPos.y),
    static_cast<_int>(vPos.z)
    };

    int _intSave;
    for (int i = 0; i < m_vecPositions.size(); ++i) {
        if (m_vecPositions[i].x == key.x &&
            m_vecPositions[i].y == key.y &&
            m_vecPositions[i].z == key.z) {
            _intSave = i;
        }
    }


    auto it = m_Colliders.find(key);
    if (it == m_Colliders.end())
        return E_FAIL;

    CCollider_Cube* pCollider = it->second;
    if (FAILED(Delete_Component(TEXT("Com_Collider_Cube"), pCollider)))
        return E_FAIL;

    if (FAILED(Drop_Item_OnDestroy(vPos)))
        return E_FAIL;

    if (FAILED(Play_Destroy_Effect(vPos)))
        return E_FAIL;

    for (size_t i = 0; i < m_vecPositions.size(); ++i)
    {
        if (m_vecPositions[i].x == vPos.x &&
            m_vecPositions[i].y == vPos.y &&
            m_vecPositions[i].z == vPos.z)
        {
            m_vecPositions.erase(m_vecPositions.begin() + i);
            m_vecBrights.erase(m_vecBrights.begin() + i);
            break;
        }
    }

    Safe_Release(pCollider);
    m_Colliders.erase(it);

    m_pVIBufferCom->Update_InstanceBuffer(m_vecPositions, m_vecBrights);

    return S_OK;
}

void CBreakableCube::Attacked_Block(_float3 vPos, int attackDamage, _float fDeltaTime)
{
    if (m_attackedBlockPos != vPos) {
        m_fHp = 100;
        //cout << "Change Block" << m_fHp << endl;
    }

    m_fHitSoundDelayAcc += fDeltaTime;

    if (m_fHitSoundDelayAcc >= m_fHitSoundDelayLimit)
    {
        PlaySound_Hit(vPos);
        m_fHitSoundDelayAcc = 0.f;
    }
    
    ITEMNAME _itemname = CUI_Mgr::Get_Instance()->GetItemTypeName();

    _wstring particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_SAND_MINING;

    switch (_itemname)
    {
    case Client::ITEMNAME_WOOD_PICKAXE:
        switch (m_itemName)
        {
        case Client::ITEMNAME_GRASSDIRT:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_DIRT:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_LEAF:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_LEAF_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_WOOD:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_OAKPLANKS:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_CRAFTINGTABLE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_STONE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 1.5;
            break;
        case Client::ITEMNAME_COBBLESTONE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 1.5;
            break;
        case Client::ITEMNAME_COALORE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 1.5;
            break;
        case Client::ITEMNAME_IRONORE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 5;
            break;
        case Client::ITEMNAME_FURANCE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 1.5;
            break;
        default:
            break;
        }
        break;
    case Client::ITEMNAME_STONE_PICKAXE:
        switch (m_itemName)
        {
        case Client::ITEMNAME_GRASSDIRT:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_DIRT:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_LEAF:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_LEAF_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_WOOD:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_OAKPLANKS:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_CRAFTINGTABLE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_STONE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        case Client::ITEMNAME_COBBLESTONE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        case Client::ITEMNAME_COALORE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        case Client::ITEMNAME_IRONORE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        case Client::ITEMNAME_FURANCE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        default:
            break;
        }
        break;
    case Client::ITEMNAME_STONE_AXE:
        switch (m_itemName)
        {
        case Client::ITEMNAME_GRASSDIRT:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_DIRT:
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_LEAF:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_LEAF_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_WOOD:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        case Client::ITEMNAME_OAKPLANKS:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        case Client::ITEMNAME_CRAFTINGTABLE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= (attackDamage / m_fHardness) * 2;
            break;
        case Client::ITEMNAME_STONE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_COBBLESTONE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_COALORE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_IRONORE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_FURANCE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= attackDamage / 5.f;
            break;
        default:
            break;
        }
        break;
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
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_LEAF_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_CRAFTINGTABLE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_WOOD:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_OAKPLANKS:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_WOOD_MINING;
            m_fHp -= attackDamage / m_fHardness;
            break;
        case Client::ITEMNAME_STONE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_COBBLESTONE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_COALORE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_IRONORE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= attackDamage / 5.f;
            break;
        case Client::ITEMNAME_FURANCE:
            particleTag = PROTOTYPE_GAMEOBJECT_PARTICLE_STONE_MINING;
            m_fHp -= attackDamage / 5.f;
            break;
        default:
            break;
        }

        break;
    }
    m_attackedBlockPos = vPos;
    m_resetHpFrame = 0;


    CParticleEventManager::Get_Instance()->OnParticle(
        particleTag,
        vPos);
}

void CBreakableCube::PlaySound_Hit(_float3 vPos)
{

    m_pGameInstance->Play_Sound(TEXT("Stone_hit1"), SOUND_BLOCK_HIT, this, 1.f, vPos);

    //printf_s("Play Sound\n");
}

float CBreakableCube::GetHP() const
{
    return m_fHp;
}

void CBreakableCube::Set_Bright()
{
    if (CUI_Mgr::Get_Instance()->GetItemTypeName() == ITEMNAME_TORCH || CUI_Mgr::Get_Instance()->GetItemTypeName_Left() == ITEMNAME_TORCH) {
        CGameObject* pSteve{ nullptr };
        pSteve = m_pGameInstance->Get_LastObject(LEVEL_YU, TEXT("Layer_Steve"));

        CTransform* pTransformCom{ nullptr };
        pTransformCom = static_cast<CTransform*>(pSteve->Find_Component(TEXT("Com_Transform")));
        _float3 vStevePos = { pTransformCom->Get_State(CTransform::STATE_POSITION) };


        for (int i = 0; i < m_vecPositions.size(); ++i) {
            // 거리 재고
            _float3 vDiff{ vStevePos - m_vecPositions[i] };
            _float fLengthSq{ D3DXVec3LengthSq(&vDiff) };

            // 가까우면 밝기 0.2 0.1씩 올려준거거든 
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
            m_vecBrights[i] = g_fBright * m_vecPositions[i].y / 8.f;
        }
    }

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

HRESULT CBreakableCube::Drop_Item_OnDestroy(const _float3& vPos)
{
    return E_NOTIMPL;
}

HRESULT CBreakableCube::Play_Destroy_Effect(const _float3& vPos)
{
    m_pGameInstance->Play_Sound(TEXT("Stone_dig2"), SOUND_BLOCK_DIG, this, 1.f, vPos);

    return S_OK;
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

    for (auto& pair : m_Colliders)
    {
        Safe_Release(pair.second);
    }

    Safe_Release(m_pShaderCom);
}
